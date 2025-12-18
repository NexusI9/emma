#ifndef _EMMA_MODULE_COMPOUND_EXPERIENCE_H_
#define _EMMA_MODULE_COMPOUND_EXPERIENCE_H_

#include "runtime/solutions/module/compounds/action.h"
#include "runtime/solutions/module/compounds/utils.h"
#include <inttypes.h>

typedef enum {
  CompoundExperienceImpact_Trivial,
  CompoundExperienceImpact_Routine,
  CompoundExperienceImpact_Important,
} CompoundExperienceImpact;

typedef enum {
  CompoundExperienceLearnability_Easy,
  CompoundExperienceLearnability_Medium,
  CompoundExperienceLearnability_Hard,
} CompoundExperienceLearnability;

typedef struct {
  CompoundExperienceImpact impact;
  CompoundExperienceLearnability learnability;
} CompoundExperience;

static inline float
compound_experience_get_impact_weight(const CompoundExperience *set) {

  static const float impact_weight[] = {
      [CompoundExperienceImpact_Trivial] = 0.15,
      [CompoundExperienceImpact_Routine] = 0.5,
      [CompoundExperienceImpact_Important] = 1,
  };

  return impact_weight[set->impact];
}

static inline float
compound_experience_get_learnability_weight(const CompoundExperience *set) {

  static const float learn_weight[] = {
      [CompoundExperienceLearnability_Easy] = 0.15,
      [CompoundExperienceLearnability_Medium] = 0.5,
      [CompoundExperienceLearnability_Hard] = 1,
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
compound_experience_get_friction(const CompoundExperience *set,
                                 const uint32_t actions_count) {

  static const float w_action = 0.60f;
  static const float w_impact = 0.25f;
  static const float w_learn = 0.15f;

  const float impact = compound_experience_get_impact_weight(set);
  const float learn = compound_experience_get_learnability_weight(set);

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
compound_experience_get_excitement(const CompoundExperience *set,
                                   const uint32_t actions_count) {

  static const float w_action = 0.60f;
  static const float w_impact = 0.25f;
  static const float w_learn = 0.15f;

  const float impact = compound_experience_get_impact_weight(set);
  const float learn = compound_experience_get_learnability_weight(set);

  // clang-format off
  return   w_action * inv_norm(actions_count, COMPOUND_ACTION_CAPACITY)
         + w_impact * (1 - impact)
         + w_learn  * (1 - learn);
  // clang-format on
}

/*
  Based on routine-to-important actions coupled with an easy learnability,
  making feel the user do meaningful choices smoothly without frictions.
 */
static inline float
compound_experience_get_empowerment(const CompoundExperience *set) {

  static const float w_impact = 0.7;
  static const float w_learn = 0.3f;

  const float impact = compound_experience_get_impact_weight(set);
  const float learn = compound_experience_get_learnability_weight(set);

  return w_impact * impact + w_learn * (1 - learn);
}

#endif
