#ifndef _VIEWPORT_MANAGER_H_
#define _VIEWPORT_MANAGER_H_

#include "nkengine/include/clock.h"
#include "nkengine/include/input.h"
#include <cglm/cglm.h>

typedef struct {
  float pan_sensitivity;
  float zoom_sensitivity;
  vec2 pan;
  float scale;
} ViewportManager;

extern ViewportManager g_viewport_manager;

EXTERN_C_BEGIN

void viewport_update(ViewportManager *);

static inline void viewport_set_pan_sensitivity(const float value) {
  g_viewport_manager.pan_sensitivity = value;
}

static inline void viewport_set_zoom_sensitivity(const float value) {
  g_viewport_manager.zoom_sensitivity = value;
}

static inline float viewport_get_scale() { return g_viewport_manager.scale; }

// x
static inline float vpx(const float value) {
  return g_viewport_manager.pan[0] + value * g_viewport_manager.scale;
}

// y
static inline float vpy(const float value) {
  return g_viewport_manager.pan[1] + value * g_viewport_manager.scale;
}

// vec2
static inline void vp2(vec2 value, vec2 dest) {
  dest[0] = g_viewport_manager.pan[0] + value[0] * g_viewport_manager.scale;
  dest[1] = g_viewport_manager.pan[1] + value[1] * g_viewport_manager.scale;
}

// to scene space
static inline float vpx_scene(const float val) {
  return (val - g_viewport_manager.pan[0]) / g_viewport_manager.scale;
}

static inline float vpy_scene(const float val) {
  return (val - g_viewport_manager.pan[1]) / g_viewport_manager.scale;
}

static inline void vp2_scene(const vec2 src, vec2 dst) {
  dst[0] = vpx_scene(src[0]);
  dst[1] = vpx_scene(src[1]);
}

EXTERN_C_END

#endif
