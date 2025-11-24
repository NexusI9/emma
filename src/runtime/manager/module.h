#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include "allocator_list.h"
#include "nkengine/include/texture.h"
#include "webgpu/webgpu.h"
#include <cglm/cglm.h>

typedef enum {
  ModuleType_News,
  ModuleType_Rate,
  ModuleType_SpinningWheel,
  ModuleType_Ranking,
  ModuleType_SurveyCheckbox,
  ModuleType_SurveySlider,
  ModuleType_COUNT,
} ModuleType;

typedef enum {
  ModuleManagerStatus_Success,
  ModuleManagerStatus_OutOfBound,
  ModuleManagerStatus_UnavailableResource,
  ModuleManagerStatus_UndefError,
} ModuleManagerStatus;

typedef struct {

  WGPUTexture atlas_texture;
  WGPUTextureView atlas_view;

} ModuleManager;

extern ModuleManager g_module_manager;

typedef struct {
  const char *label;
  const vec2 uv0, uv1;
  const vec2 size;
} ModuleDescriptor;

EXTERN_C_BEGIN

ModuleManagerStatus module_manager_create_texture(const char *,
                                                  const TextureResolution);

ModuleManagerStatus module_manager_init_default_modules();

const ModuleDescriptor *module_manager_get_module(const ModuleType);

static inline WGPUTextureView module_view() {
  return g_module_manager.atlas_view;
}

EXTERN_C_END

#endif
