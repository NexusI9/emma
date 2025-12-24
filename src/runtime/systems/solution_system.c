#include "solution_system.h"

#include "runtime/node/frame.h"
#include "runtime/node/motivation.h"
#include "runtime/solutions/formula.h"
#include "runtime/solutions/module/core.h"
#include "runtime/solutions/persona/core.h"
#include "runtime/solutions/utils.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>

/*
  To combine Module and Persona solution we use segment solution elements into
  two category depending on Persona's resonance towards the target element.

  1. Desire Based Elements (DBE): Only scale the result positively, meaning if
  persona has a low resonance with epic meaning and the module has a high epic
  meaning, it will lower the score without going below zero. The reason is that
  DBEs won't affect the experience negatively if the user isn't interested in
  them.

  { Epic Meaning, Empowerment, Accomplishment, Ownership }


  2. Aversion Based Element (ABE): Scale the result if persona resonate with it,
  but also adds penalty if persona isn't fond of it. As instance if persona has
  a low score of Unpredictability, meaning they avoid risk and prefer stable
  outcome; if a module has a high unpredictability, this will negavitaly impact
  the user experience as it may causes stress or additional anxienty to the
  user.

  { Scarcity, Avoidance, Unpredictability, Social Influence }
 */
float solution_system_module_persona_element(const Solution *module,
                                             const Solution *persona,
                                             const MotivationType element) {

  static const float w_persona = 0.7f;
  static const float w_module = (1 - w_persona);

  const float m_value = 0; // solution_core_get_element(module, element);
  float p_value = 0;       // solution_core_get_element(&module->core, element);

  switch (element) {

    // Aversion Based Elements (Penalty)
  case MotivationType_SocialInfluence:
  case MotivationType_Unpredictability:
  case MotivationType_Avoidance:
  case MotivationType_Scarcity:
  case MotivationType_Friction:
  case MotivationType_SocialBounding:
  case MotivationType_Reward:
    p_value = polarize(p_value); // map to [-1, 1]
    break;

  default:
    break;
  }

  const float formula = w_module * m_value + w_persona * p_value;

  // clamp to 0
  return fmaxf(0.0f, formula);
}

/*
  Traverse all the frame child module and average modules values.
 */
void solution_system_update_frame(Solution *solution, Frame *frame) {

  for (size_t i = 0; i < frame->children.count; i++) {
  }
}

void solution_system_update_motivation(const Solution *solution,
                                       Motivation *motivation) {

  // Retrieve the solution formulas set according to its type (Module, Persona)
  const solution_formula *formulas = solution_get_formulas(solution->type);

  // Call each formulas and update the motivation data
  for (int i = 0; i < MotivationType_COUNT; i++)
    if (formulas[i])
      motivation_set_element(motivation, (MotivationType)i,
                             formulas[i](solution));
}
