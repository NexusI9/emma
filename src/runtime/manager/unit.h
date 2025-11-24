#ifndef _UNIT_MANAGER_H_
#define _UNIT_MANAGER_H_

#include <cglm/cglm.h>

typedef struct {
  float step;
} UnitManager;

extern UnitManager g_unit_manager;

static inline void unit_set_step(const float value) {
  g_unit_manager.step = value;
}

static inline float unit_step() { return g_unit_manager.step; }

static inline float unit_snap(const float value) {
  return roundf(value / unit_step()) * unit_step();
}

static inline void unit_snap_vec2(vec2 value) {
  value[0] = unit_snap(value[0]);
  value[1] = unit_snap(value[1]);
}

#endif
