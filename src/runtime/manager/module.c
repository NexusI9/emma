#include "module.h"
#include "./allocator.h"
#include "nkengine/include/resource_manager.h"
#include "runtime/node/frame.h"
#include "webgpu/webgpu.h"

const TextureAtlasRegion *get_module(const ModuleType type) {

  static const TextureAtlasRegion default_modules[ModuleType_COUNT] = {
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
