#ifndef _EMMA_SOLUTIONS_UTILS_H_
#define _EMMA_SOLUTIONS_UTILS_H_

#include "runtime/node/motivation.h"
#include "runtime/solutions/module/formulas.h"
#include "runtime/solutions/persona/formulas.h"
#include "runtime/solutions/solution.h"

// TODO reorga the whole formula stuff, feels weird to have one part in the
// solution.h, another in module/formula, and also here..

// clang-format off
#define SOLUTION_FORMULAS_LOOKUP(ConstLabel, FuncLabel)                                     \
  static const solution_formula ConstLabel[MotivationType_COUNT] =                         \
  {                                                                                         \
    [ MotivationType_EpicMeaning      ]  =   solution_##FuncLabel##_get_epic_meaning,      \
    [ MotivationType_Empowerment      ]  =   solution_##FuncLabel##_get_empowerment,       \
    [ MotivationType_SocialInfluence  ]  =   solution_##FuncLabel##_get_social_influence,  \
    [ MotivationType_Unpredictability ]  =   solution_##FuncLabel##_get_unpredictability,  \
    [ MotivationType_Avoidance        ]  =   solution_##FuncLabel##_get_avoidance,         \
    [ MotivationType_Scarcity         ]  =   solution_##FuncLabel##_get_scarcity,          \
    [ MotivationType_Ownership        ]  =   solution_##FuncLabel##_get_ownership,         \
    [ MotivationType_Accomplishment   ]  =   solution_##FuncLabel##_get_accomplishment,    \
    [ MotivationType_Excitment        ]  =   solution_##FuncLabel##_get_excitment,         \
    [ MotivationType_SocialBounding   ]  =   solution_##FuncLabel##_get_social_bounding,   \
    [ MotivationType_Friction         ]  =   solution_##FuncLabel##_get_friction,          \
    [ MotivationType_Reward           ]  =   solution_##FuncLabel##_get_reward,            \
 };
// clang-format on

SOLUTION_FORMULAS_LOOKUP(SOLUTION_MODULE_FORMULA, module)
SOLUTION_FORMULAS_LOOKUP(SOLUTION_PERSONA_FORMULA, persona)

/*
  Provide the right set of callback depending on the given type.
  Used when we need to update the motivation list according to a solution
  (module, persona)
 */
static inline const solution_formula *
solution_get_formulas(const SolutionType type) {

  static const solution_formula *formulas[SolutionType_COUNT] = {
      [SolutionType_Module] = SOLUTION_MODULE_FORMULA,
      [SolutionType_Persona] = SOLUTION_PERSONA_FORMULA,
  };

  return formulas[type];
}

#endif
