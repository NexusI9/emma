#ifndef _VIEWPORT_MANAGER_H_
#define _VIEWPORT_MANAGER_H_

#include "nkengine/include/clock.h"
#include "nkengine/include/input.h"
#include <cglm/cglm.h>

typedef struct {
  float sensitivity;
  vec2 pan;
} ViewportManager;

extern ViewportManager g_viewport_manager;

EXTERN_C_BEGIN

void viewport_update(ViewportManager *);

static inline void viewport_set_sensitivity(const float value) {
  g_viewport_manager.sensitivity = value;
}

static inline float vpx(const float value) {
  return g_viewport_manager.pan[0] + value;
}

static inline float vpy(const float value) {
  return g_viewport_manager.pan[1] + value;
}


EXTERN_C_END

#endif
