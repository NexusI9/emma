#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include "nkengine/include/texture.h"

typedef enum {
  ModuleType_News,
  ModuleType_Rate,
  ModuleType_SpinningWheel,
  ModuleType_Ranking,
  ModuleType_SurveyCheckbox,
  ModuleType_SurveySlider,
  ModuleType_COUNT,
} ModuleType;

EXTERN_C_BEGIN

const TextureAtlasRegion *get_module(const ModuleType);

EXTERN_C_END

#endif
