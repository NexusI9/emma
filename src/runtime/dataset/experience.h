#ifndef _EMMA_DATASET_EXPERIENCE_H_
#define _EMMA_DATASET_EXPERIENCE_H_

#include "runtime/dataset/action.h"
#include "runtime/dataset/utils.h"
#include <inttypes.h>

typedef enum {
  DataSetExperienceImpact_Trivial,
  DataSetExperienceImpact_Routine,
  DataSetExperienceImpact_Important,
} DataSetExperienceImpact;

typedef enum {
  DataSetExperienceLearnability_Easy,
  DataSetExperienceLearnability_Medium,
  DataSetExperienceLearnability_Hard,
} DataSetExperienceLearnability;

typedef struct {
  DataSetExperienceImpact impact;
  DataSetExperienceLearnability learnability;
} DataSetExperience;

static inline float
dataset_experience_get_impact_weight(const DataSetExperience *set) {

  static const float impact_weight[] = {
      [DataSetExperienceImpact_Trivial] = 0.15,
      [DataSetExperienceImpact_Routine] = 0.5,
      [DataSetExperienceImpact_Important] = 1,
  };

  return impact_weight[set->impact];
}

static inline float
dataset_experience_get_learnability_weight(const DataSetExperience *set) {

  static const float learn_weight[] = {
      [DataSetExperienceLearnability_Easy] = 0.15,
      [DataSetExperienceLearnability_Medium] = 0.5,
      [DataSetExperienceLearnability_Hard] = 1,
  };

  return learn_weight[set->learnability];
}

/*
  Friction is foremost based on the number of action available in a module,
  more actions means more complexity and potential boredom and annoyance. A
  high amount of action often also means a hard learnability. Finally if an
  action is important it will also slightly add pressure and friction.
 */
static inline float
dataset_experience_get_friction(const DataSetExperience *set,
                                const uint32_t actions_count) {

  static const float w_action = 0.60f;
  static const float w_impact = 0.25f;
  static const float w_learn = 0.15f;

  const float impact = dataset_experience_get_impact_weight(set);
  const float learn = dataset_experience_get_learnability_weight(set);

  // clang-format off
  return   w_action * actions_count
         + w_impact * impact
         + w_learn  * learn;
  // clang-format on
}

/*
  Based on low action amount, trivial impact along with an easy learnability.
  This will be coupled with average actions excitement value (based on actions’
  rewards)
 */
static inline float
dataset_experience_get_excitement(const DataSetExperience *set,
                                  const uint32_t actions_count) {

  static const float w_action = 0.60f;
  static const float w_impact = 0.25f;
  static const float w_learn = 0.15f;

  const float impact = dataset_experience_get_impact_weight(set);
  const float learn = dataset_experience_get_learnability_weight(set);

  // clang-format off
  return   w_action * inv_norm(actions_count, DATASET_ACTION_CAPACITY)
         + w_impact * (1 - impact)
         + w_learn  * (1 - learn);
  // clang-format on
}

#endif
