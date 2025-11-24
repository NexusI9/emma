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

  Texture texture;
  texture_create_from_file(&texture, &(TextureCreateFileDescriptor){
                                         .path = path,
                                         .channels = TextureChannel_RGBA,
                                         .flip = false,
                                         .width = resolution,
                                         .height = resolution,
                                     });

  rem_write_texture(g_module_manager.atlas_texture, texture.data, texture.size,
                    texture.channels, 0, REMWriteFlag_STBIFreeData);

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

const ModuleDescriptor *module_manager_get_module(const ModuleType type) {

  static const ModuleDescriptor default_modules[ModuleType_COUNT] = {
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

  return &default_modules[type];
}
