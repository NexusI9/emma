#include "heatmap.h"
#include "nkengine/include/compute.h"
#include "nkengine/include/context.h"
#include "nkengine/include/resource_manager.h"
#include "webgpu/webgpu.h"
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
