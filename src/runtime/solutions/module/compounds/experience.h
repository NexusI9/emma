#ifndef _EMMA_MODULE_COMPOUND_EXPERIENCE_H_
#define _EMMA_MODULE_COMPOUND_EXPERIENCE_H_

#include "runtime/solutions/utils.h"
#include "runtime/solutions/module/compounds/action.h"
#include <inttypes.h>

typedef enum {
  CompoundModuleExperienceImpact_Trivial,
  CompoundModuleExperienceImpact_Routine,
  CompoundModuleExperienceImpact_Important,
} CompoundModuleExperienceImpact;

typedef enum {
  CompoundModuleExperienceLearnability_Easy,
  CompoundModuleExperienceLearnability_Medium,
  CompoundModuleExperienceLearnability_Hard,
} CompoundModuleExperienceLearnability;

typedef struct {
  CompoundModuleExperienceImpact impact;
  CompoundModuleExperienceLearnability learnability;
} CompoundModuleExperience;

static inline float compound_module_experience_get_impact_weight(
    const CompoundModuleExperience *set) {

  static const float impact_weight[] = {
      [CompoundModuleExperienceImpact_Trivial] = 0.15,
      [CompoundModuleExperienceImpact_Routine] = 0.5,
      [CompoundModuleExperienceImpact_Important] = 1,
  };

  return impact_weight[set->impact];
}

static inline float compound_module_experience_get_learnability_weight(
    const CompoundModuleExperience *set) {

  static const float learn_weight[] = {
      [CompoundModuleExperienceLearnability_Easy] = 0.15,
      [CompoundModuleExperienceLearnability_Medium] = 0.5,
      [CompoundModuleExperienceLearnability_Hard] = 1,
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
compound_module_experience_get_friction(const CompoundModuleExperience *set,
                                        const uint32_t actions_count) {

  static const float w_action = 0.60f;
  static const float w_impact = 0.25f;
  static const float w_learn = 0.15f;

  const float impact = compound_module_experience_get_impact_weight(set);
  const float learn = compound_module_experience_get_learnability_weight(set);

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
compound_module_experience_get_excitement(const CompoundModuleExperience *set,
                                          const uint32_t actions_count) {

  static const float w_action = 0.60f;
  static const float w_impact = 0.25f;
  static const float w_learn = 0.15f;

  const float impact = compound_module_experience_get_impact_weight(set);
  const float learn = compound_module_experience_get_learnability_weight(set);

  // clang-format off
  return   w_action * inv_norm(actions_count, COMPOUND_MODULE_ACTION_CAPACITY)
         + w_impact * (1 - impact)
         + w_learn  * (1 - learn);
  // clang-format on
}

/*
  Based on routine-to-important actions coupled with an easy learnability,
  making feel the user do meaningful choices smoothly without frictions.
 */
static inline float compound_module_experience_get_empowerment(
    const CompoundModuleExperience *set) {

  static const float w_impact = 0.7;
  static const float w_learn = 0.3f;

  const float impact = compound_module_experience_get_impact_weight(set);
  const float learn = compound_module_experience_get_learnability_weight(set);

  return w_impact * impact + w_learn * (1 - learn);
}

#endif
