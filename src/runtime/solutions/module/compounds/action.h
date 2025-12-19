#ifndef _EMMA_MODULE_COMPOUND_ACTION_H_
#define _EMMA_MODULE_COMPOUND_ACTION_H_

#include <inttypes.h>

static const uint8_t COMPOUND_MODULE_ACTION_CAPACITY = 6;

typedef enum {
  CompoundModuleActionRole_Primary,
  CompoundModuleActionRole_Secondary,
  CompoundModuleActionRole_Tertiary,
} CompoundModuleActionRole;

typedef struct {
  const char *label;
  CompoundModuleActionRole role;
} CompoundModuleAction;

static inline float
compound_module_action_get_role_weight(const CompoundModuleAction *action) {

  static const float role_weight[] = {
      [CompoundModuleActionRole_Primary] = 1,
      [CompoundModuleActionRole_Secondary] = 0.5,
      [CompoundModuleActionRole_Tertiary] = 0.15,
  };

  return role_weight[action->role];
}

#endif
