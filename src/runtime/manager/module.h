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
  const char *label;
  const vec2 uv0, uv1;
  const vec2 size;
} ModuleDescriptor;

EXTERN_C_BEGIN

const ModuleDescriptor *get_module(const ModuleType);


EXTERN_C_END

#endif
