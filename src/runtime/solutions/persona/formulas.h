#ifndef _EMMA_SOLUTION_PERSONA_FORMULAS_H_
#define _EMMA_SOLUTION_PERSONA_FORMULAS_H_

#include "../solution.h"

SOLUTION_FORMULAS_DEF(persona)

float solution_persona_get_epic_meaning(const Solution *solution) {

  return compound_persona_motivation_get_epic_meaning(
      &solution->persona.motivation);
}

float solution_persona_get_empowerment(const Solution *solution) {

  static const float w_agency = 0.4f;
  static const float w_motivation = 0.6f;

  // clang-format off
  return w_motivation * compound_persona_motivation_get_empowerment(&solution->persona.motivation)
       + w_agency * compound_persona_agency_get_empowerment(&solution->persona.agency);
  // clang-format on
}

float solution_persona_get_social_influence(const Solution *solution) {

  static const float w_motivation = 0.6f;
  static const float w_social = 0.4f;

  // clang-format off
  return w_motivation * compound_persona_motivation_get_social_influence(&solution->persona.motivation)
       + w_social * compound_persona_social_get_influence(&solution->persona.social);
  // clang-format on
}

float solution_persona_get_unpredictability(const Solution *solution) {

  static const float w_motivation = 0.6f;
  static const float w_agency = 0.4f;

  // clang-format off
  return w_motivation * compound_persona_motivation_get_unpredictability(&solution->persona.motivation)
       + w_agency * compound_persona_agency_get_unpredictability(&solution->persona.agency);
  // clang-format on
}

float solution_persona_get_avoidance(const Solution *solution) {

  static const float w_motivation = 0.6f;
  static const float w_agency = 0.4f;

  // clang-format off
  return w_motivation * compound_persona_motivation_get_avoidance(&solution->persona.motivation)
       + w_agency * compound_persona_agency_get_avoidance(&solution->persona.agency);
  // clang-format on
}

float solution_persona_get_scarcity(const Solution *solution) {

  return compound_persona_motivation_get_scarcity(
      &solution->persona.motivation);
}

float solution_persona_get_ownership(const Solution *solution) {

  return compound_persona_motivation_get_ownership(
      &solution->persona.motivation);
}

float solution_persona_get_accomplishment(const Solution *solution) {

  static const float w_motivation = 0.6f;
  static const float w_agency = 0.4f;

  // clang-format off
  return w_motivation * compound_persona_motivation_get_accomplishment(&solution->persona.motivation)
       + w_agency * compound_persona_agency_get_accomplishment(&solution->persona.agency);
  // clang-format on
}

float solution_persona_get_excitement(const Solution *solution) {

  return compound_persona_motivation_get_excitement(
      &solution->persona.motivation);
}

float solution_persona_get_social_bounding(const Solution *solution) {

  static const float w_motivation = 0.4f;
  static const float w_social = 0.6f;

  // clang-format off
  return w_motivation * compound_persona_motivation_get_social_bounding(&solution->persona.motivation)
       + w_social * compound_persona_social_get_bounding(&solution->persona.social);
  // clang-format on
}

float solution_persona_get_friction(const Solution *solution) {

  static const float w_motivation = 0.4f;
  static const float w_agency = 0.6f;

  // clang-format off
  return w_motivation * compound_persona_motivation_get_friction(&solution->persona.motivation)
       + w_agency * compound_persona_agency_get_friction(&solution->persona.agency);
  // clang-format on
}

float solution_persona_get_reward(const Solution *solution) {

  static const float w_motivation = 0.4f;
  static const float w_agency = 0.6f;

  // clang-format off
  return w_motivation * compound_persona_motivation_get_reward(&solution->persona.motivation)
       + w_agency * compound_persona_agency_get_reward(&solution->persona.agency);
  // clang-format on
}

#endif
