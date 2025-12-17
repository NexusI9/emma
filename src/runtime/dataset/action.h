#ifndef _EMMA_DATASET_ACTION_H_
#define _EMMA_DATASET_ACTION_H_

#include <inttypes.h>

static const uint8_t DATASET_ACTION_CAPACITY = 6;

typedef enum {
  DataSetActionRole_Primary,
  DataSetActionRole_Secondary,
  DataSetActionRole_Tertiary,
} DataSetActionRole;

typedef struct {
  DataSetActionRole role;
} DataSetAction;

static inline float dataset_action_role_weight(const DataSetAction *action) {

  static const float role_weight[] = {
      [DataSetActionRole_Primary] = 1,
      [DataSetActionRole_Secondary] = 0.5,
      [DataSetActionRole_Tertiary] = 0.15,
  };

  return role_weight[action->role];
}

#endif
