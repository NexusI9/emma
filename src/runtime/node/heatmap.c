#include "heatmap.h"
#include "nkengine/include/resource_manager.h"
#include "webgpu/webgpu.h"
#include <stdint.h>

HeatmapStatus heatmap_create(Heatmap *map, const HeatmapDescriptor *desc) {

  glm_vec4_copy((float *)desc->background, map->background);
  map->frames = desc->frames;
  map->intensity_mapper = desc->intensity_mapper;
  
  map->texture = rem_new_texture(&(WGPUTextureDescriptor){
      .dimension = WGPUTextureDimension_2D,
      .format = TEXTURE_FORMAT_OFFSCREEN,
      .label = "Heat map texture",
      .sampleCount = 1,
      .mipLevelCount = 1,
      .usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_RenderAttachment,
      .size = {desc->width, desc->height, 1},
  });

  map->views[0] =
      rem_new_view(map->texture, &(WGPUTextureViewDescriptor){
                                     .arrayLayerCount = 1,
                                     .baseArrayLayer = 0,
                                     .mipLevelCount = 1,
                                     .aspect = WGPUTextureAspect_All,
                                     .format = TEXTURE_FORMAT_OFFSCREEN,
                                     .label = "Heat map texture view",
                                     .dimension = WGPUTextureViewDimension_2D,
                                 });



  return HeatmapStatus_Success;
}
