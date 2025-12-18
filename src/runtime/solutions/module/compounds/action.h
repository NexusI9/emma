#ifndef _EMMA_MODULE_COMPOUND_ACTION_H_
#define _EMMA_MODULE_COMPOUND_ACTION_H_

#include <inttypes.h>

static const uint8_t COMPOUND_ACTION_CAPACITY = 6;

typedef enum {
  CompoundActionRole_Primary,
  CompoundActionRole_Secondary,
  CompoundActionRole_Tertiary,
} CompoundActionRole;

typedef struct {
  const char *label;
  CompoundActionRole role;
} CompoundAction;

static inline float compound_action_get_role_weight(const CompoundAction *action) {

  static const float role_weight[] = {
      [CompoundActionRole_Primary] = 1,
      [CompoundActionRole_Secondary] = 0.5,
      [CompoundActionRole_Tertiary] = 0.15,
  };

  return role_weight[action->role];
}

#endif
