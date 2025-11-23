#include "module.h"
#include "./allocator.h"
#include "nkengine/include/resource_manager.h"
#include "runtime/node/frame.h"
#include "webgpu/webgpu.h"

ModuleManager g_module_manager = {0};

ModuleManagerStatus
module_manager_create_texture(const char *path,
                              const TextureResolution resolution) {

  g_module_manager.atlas_texture = rem_new_texture(&(WGPUTextureDescriptor){
      .label = "Module Atlas Texture",
      .dimension = WGPUTextureDimension_2D,
      .format = TEXTURE_FORMAT_OFFSCREEN,
      .mipLevelCount = 1,
      .sampleCount = 1,
      .size = {resolution, resolution, 1},
      .usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding,
  });

  g_module_manager.atlas_view =
      rem_new_view(g_module_manager.atlas_texture,
                   &(WGPUTextureViewDescriptor){
                       .label = "Module Atlas Texture View",
                       .arrayLayerCount = 1,
                       .baseArrayLayer = 0,
                       .mipLevelCount = 1,
                       .aspect = WGPUTextureAspect_All,
                       .dimension = WGPUTextureViewDimension_2D,
                   });

  return ModuleManagerStatus_Success;
}

ModuleManagerStatus
module_manager_register_module(const ModuleType type,
                               const ModuleDescriptor *desc) {

  if (type > ModuleType_COUNT) { // ERRHANDLE
    return ModuleManagerStatus_OutOfBound;
  }

  Frame *module = new_frame();

  if (!module) // ERRHANDLE
    return ModuleManagerStatus_UnavailableResource;

  frame_create(module, &(FrameDescriptor){
                           .label = desc->label,
                           .size = {desc->size[0], desc->size[1]},
                           .uv0 = {desc->uv0[0], desc->uv0[1]},
                           .uv1 = {desc->uv1[0], desc->uv1[1]},
                       });

  return ModuleManagerStatus_Success;
}

ModuleManagerStatus module_manager_init_default_modules() {

  static ModuleDescriptor default_modules[ModuleType_COUNT] = {
      [ModuleType_News] =
          {
              .label = "Module News",
              .uv0 = {0.0f / 2048.0f, 0.0f / 2048.0f},
              .uv1 = {360.0f / 2048.0f, 460.0f / 2048.0f},
              .size = {360.0f, 460.0f},
          },
      [ModuleType_Rate] =
          {
              .label = "Module Rate",
              .uv0 = {360.0f / 2048.0f, 0.0f / 2048.0f},
              .uv1 = {(360.0f + 360.0f) / 2048.0f, 400.0f / 2048.0f},
              .size = {360.0f, 400.0f},
          },
      [ModuleType_SpinningWheel] =
          {
              .label = "Module Spinning Wheel",
              .uv0 = {720.0f / 2048.0f, 0.0f / 2048.0f},
              .uv1 = {(720.0f + 360.0f) / 2048.0f, 660.0f / 2048.0f},
              .size = {360.0f, 660.0f},
          },
      [ModuleType_Ranking] =
          {
              .label = "Module Ranking",
              .uv0 = {1080.0f / 2048.0f, 0.0f / 2048.0f},
              .uv1 = {(1080.0f + 360.0f) / 2048.0f, 320.0f / 2048.0f},
              .size = {360.0f, 320.0f},
          },
      [ModuleType_SurveyCheckbox] =
          {
              .label = "Module Survey Checkbox A",
              .uv0 = {1080.0f / 2048.0f, 320.0f / 2048.0f},
              .uv1 = {(1080.0f + 360.0f) / 2048.0f,
                      (320.0f + 200.0f) / 2048.0f},
              .size = {360.0f, 200.0f},
          },
      [ModuleType_SurveySlider] =
          {
              .label = "Module Survey Checkbox B",
              .uv0 = {1080.0f / 2048.0f, 520.0f / 2048.0f},
              .uv1 = {(1080.0f + 360.0f) / 2048.0f,
                      (520.0f + 200.0f) / 2048.0f},
              .size = {360.0f, 200.0f},
          },
  };

  for (uint8_t i = 0; i < ModuleType_COUNT; i++)
    module_manager_register_module((ModuleType)i, &default_modules[i]);

  return ModuleManagerStatus_Success;
}
