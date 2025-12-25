#include "heatmap.h"
#include "nkengine/include/compute.h"
#include "nkengine/include/context.h"
#include "nkengine/include/resource_manager.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/node/motivation.h"
#include "utils/id.h"
#include "webgpu/webgpu.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>

static inline void heatmap_create_texture(WGPUTexture *, WGPUTextureView *,
                                          const uint32_t, const uint32_t,
                                          const WGPUTextureFormat,
                                          const WGPUTextureUsageFlags);

HeatmapStatus heatmap_create(Heatmap *map, const HeatmapDescriptor *desc) {

  glm_vec4_copy((float *)desc->background, map->background);
  map->frames = desc->frames;
  map->intensity_mapper = desc->intensity_mapper;
  map->blur = desc->blur;
  map->scale = fmaxf(0.1f, desc->scale);
  map->label = desc->label;
  map->axes[0] = desc->axes[0];
  map->axes[1] = desc->axes[1];
  map->color_map = *desc->color_map;
  map->motivation_type = desc->motivation;

  // === On screen texture (BGRA): used to render imgui ===
  heatmap_create_texture(&map->textures[HeatmapTexture_Color_Onscreen],
                         &map->views[HeatmapTexture_Color_Onscreen],
                         desc->width, desc->height, TEXTURE_FORMAT_ONSCREEN,
                         WGPUTextureUsage_CopyDst |
                             WGPUTextureUsage_TextureBinding |
                             WGPUTextureUsage_RenderAttachment);

  // === Off screen texture (RGBA): used for compute passes ===
  heatmap_create_texture(
      &map->textures[HeatmapTexture_Color_Offscreen],
      &map->views[HeatmapTexture_Color_Offscreen], desc->width * map->scale,
      desc->height * map->scale, TEXTURE_FORMAT_OFFSCREEN,
      WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding |
          WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_StorageBinding);

  // === Define compute shaders ===
  compute_pass_kawase_create(
      &map->compute_passes[HeatmapComputePass_Kawase],
      &(ComputePassDescriptor){
          .label = "Heatmap Kawase",
          .source_texture = map->textures[HeatmapTexture_Color_Offscreen],
      });

  compute_pass_colormap_create(
      &map->compute_passes[HeatmapComputePass_Colormap],
      &(ComputePassDescriptor){
          .label = "Heatmap Remap",
          .source_texture = map->textures[HeatmapTexture_Color_Offscreen],
      });

  compute_pass_colormap_set_colors(
      &map->compute_passes[HeatmapComputePass_Colormap], desc->color_map);

  compute_pass_bgra2rgba_create(
      &map->compute_passes[HeatmapComputePass_BGRA2RGBA],
      &(ComputePassDescriptor){
          .label = "Heatmap BGRA to RGBA",
          .source_texture = map->textures[HeatmapTexture_Color_Onscreen],
          .buffer_texture = map->textures[HeatmapTexture_Color_Offscreen],
      });

  // === Depth texture: Needed by imgui ===
  gui_create_depth_texture(desc->width, desc->height,
                           &map->textures[HeatmapTexture_Depth],
                           &map->views[HeatmapTexture_Depth]);

  return HeatmapStatus_Success;
}

static inline void heatmap_create_texture(WGPUTexture *texture,
                                          WGPUTextureView *view,
                                          const uint32_t width,
                                          const uint32_t height,
                                          const WGPUTextureFormat format,
                                          const WGPUTextureUsageFlags usage) {

  *texture = rem_new_texture(&(WGPUTextureDescriptor){
      .dimension = WGPUTextureDimension_2D,
      .format = format,
      .label = "Heat map texture",
      .sampleCount = 1,
      .mipLevelCount = 1,
      .usage = usage,
      .size = {width, height, 1},
  });

  *view = rem_new_view(*texture, &(WGPUTextureViewDescriptor){
                                     .arrayLayerCount = 1,
                                     .baseArrayLayer = 0,
                                     .mipLevelCount = 1,
                                     .aspect = WGPUTextureAspect_All,
                                     .format = format,
                                     .label = "Heat map texture view",
                                     .dimension = WGPUTextureViewDimension_2D,
                                 });
}

/**
   It's important to note that heatmap tones are based on frames Relative
   values, meaning the require their own Context to display the correct tone.

   A clearer example is: for each frame instead of using a global max excitement
   value (often being 1), we will use the maximum excitement value present in
   the heatmap frame list.

   This will ensure seing which frame has the most excitement AMONGST the
   registered frames.

   As a result we need to cache the max value of each motivations/
   heatmap attributes (frictions, excitemeent etc...)
 */
HeatmapStatus heatmap_update_relative_motivations(Heatmap *heatmap) {

  // === Adjust capacity ===

  if (heatmap->relative_motivations.count < heatmap->frames->count) {

    // use for instead of while to prevent infinite loop if push fails
    for (size_t i = heatmap->relative_motivations.count;
         i < heatmap->frames->count; i++)
      allocator_id_list_push(
          heatmap->relative_motivations.entries, ALLOCATOR_FRAME_CAPACITY,
          &heatmap->relative_motivations.count, new_motivation()->id);

  } else {

    // destroy overflow motivations
    while (heatmap->relative_motivations.count > heatmap->frames->count)
      allocator_id_list_pop(
          heatmap->relative_motivations.entries,
          &heatmap->relative_motivations.count,
          heatmap->relative_motivations
              .entries[heatmap->relative_motivations.count - 1]);
  }

  // === Get max values ===
  float max_value = 0.0f;

  for (size_t i = 0; i < heatmap->frames->count; i++) {
    const Frame *module = allocator_frame_entry(heatmap->frames->entries[i]);

    const Motivation *source_motivation = allocator_motivation_entry(
        module->motivations.entries[FrameMotivations_Default]);

    const float source_value =
        motivation_get_element(source_motivation, heatmap->motivation_type);

    max_value = fmaxf(max_value, source_value);
  }

  // === Cache relative motivations ===
  for (size_t i = 0; i < heatmap->frames->count; i++) {
    const Frame *module = allocator_frame_entry(heatmap->frames->entries[i]);

    const Motivation *source_motivation = allocator_motivation_entry(
        module->motivations.entries[FrameMotivations_Default]);

    Motivation *motivation =
        allocator_motivation_entry(heatmap->relative_motivations.entries[i]);

    const float source_value =
        motivation_get_element(source_motivation, heatmap->motivation_type);

    float relative_value = 0.0f;

    if (max_value != 0.0f)
      relative_value = fminf(1.0f, fmaxf(0.0f, source_value / max_value));

    motivation_set_element(motivation, heatmap->motivation_type,
                           relative_value);
  }

  return HeatmapStatus_Success;
}
