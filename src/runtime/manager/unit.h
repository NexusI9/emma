#ifndef _UNIT_MANAGER_H_
#define _UNIT_MANAGER_H_

typedef struct {
  float step;
} UnitManager;

extern UnitManager g_unit_manager;

static inline void unit_set_step(const float value) {
  g_unit_manager.step = value;
}

#endif
