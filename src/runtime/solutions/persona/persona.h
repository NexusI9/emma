#ifndef _EMMA_SOLUTION_PERSONA_H_
#define _EMMA_SOLUTION_PERSONA_H_

/*
  In the overall engine, Persona Solution act as a counter balance to the added
  Module solution. Meaning each persona will have bonus or malus for each
  solution Core, since each persona has preferences and weakness based on their
  character and emotions.

  As instance the giver will be highly concerned about the social influence and
  bounding of an app, as sharing is social interaction is often something they
  crave for. Then the Pioneer will be more keen to Empowerment and
  Self-Accomplishment.
 */

#include "runtime/node/persona.h"
#include "runtime/solutions/core.h"
#include "runtime/solutions/persona/compounds/agency.h"
#include "runtime/solutions/persona/compounds/density.h"
#include "runtime/solutions/persona/compounds/motivation.h"
#include "runtime/solutions/persona/compounds/social.h"

typedef struct {

  CompoundPersonaSocial social;
  CompoundPersonaMotivation motivation;
  CompoundPersonaAgency agency;
  CompoundPersonaDensity density;

} SolutionPersonaCompounds;

typedef struct {
  SolutionCore core;
  SolutionPersonaCompounds compounds;
} SolutionPersona;

static inline void
solution_persona_update_epic_meaning(SolutionPersona *solution) {
  solution->core.epic_meaning = compound_persona_motivation_get_epic_meaning(
      &solution->compounds.motivation);
}

static inline void
solution_persona_update_empowerment(SolutionPersona *solution) {

  static const float w_agency = 0.4f;
  static const float w_motivation = 0.6f;

  // clang-format off
  solution->core.empowerment =
        w_motivation * compound_persona_motivation_get_empowerment(&solution->compounds.motivation)
      + w_agency * compound_persona_agency_get_empowerment(&solution->compounds.agency);
  // clang-format on
}

static inline void
solution_persona_update_social_influence(SolutionPersona *solution) {

  static const float w_motivation = 0.6f;
  static const float w_social = 0.4f;

  // clang-format off
  solution->core.social_influence =
       w_motivation * compound_persona_motivation_get_social_influence(&solution->compounds.motivation)
     + w_social * compound_persona_social_get_influence(&solution->compounds.social);
  // clang-format on
}

static inline void
solution_persona_update_upredictability(SolutionPersona *solution) {

  static const float w_motivation = 0.6f;
  static const float w_agency = 0.4f;

  // clang-format off
  solution->core.unpredictability =
      w_motivation * compound_persona_motivation_get_unpredictability(&solution->compounds.motivation)
    + w_agency * compound_persona_agency_get_unpredictability(&solution->compounds.agency);
  // clang-format on
}

static inline void
solution_persona_update_avoidance(SolutionPersona *solution) {

  static const float w_motivation = 0.6f;
  static const float w_agency = 0.4f;

  // clang-format off
  solution->core.avoidance =
      w_motivation * compound_persona_motivation_get_avoidance(&solution->compounds.motivation)
    + w_agency * compound_persona_agency_get_avoidance(&solution->compounds.agency);
  // clang-format on
}

static inline void solution_persona_update_scarcity(SolutionPersona *solution) {
  solution->core.scarcity =
      compound_persona_motivation_get_scarcity(&solution->compounds.motivation);
}

static inline void
solution_persona_update_ownership(SolutionPersona *solution) {
  solution->core.ownership = compound_persona_motivation_get_ownership(
      &solution->compounds.motivation);
}

static inline void
solution_persona_update_accomplishment(SolutionPersona *solution) {

  static const float w_motivation = 0.6f;
  static const float w_agency = 0.4f;

  // clang-format off
  solution->core.accomplishment =
      w_motivation * compound_persona_motivation_get_accomplishment(&solution->compounds.motivation)
    + w_agency * compound_persona_agency_get_accomplishment(&solution->compounds.agency);
  // clang-format on
}

static inline void
solution_persona_update_excitment(SolutionPersona *solution) {
  solution->core.excitment = compound_persona_motivation_get_excitment(
      &solution->compounds.motivation);
}

static inline void
solution_persona_update_social_bounding(SolutionPersona *solution) {

  static const float w_motivation = 0.4f;
  static const float w_social = 0.6f;

  // clang-format off
  solution->core.social_bounding =
       w_motivation * compound_persona_motivation_get_social_bounding(&solution->compounds.motivation)
     + w_social * compound_persona_social_get_bounding(&solution->compounds.social);
  // clang-format on
}

static inline void solution_persona_update_friction(SolutionPersona *solution) {

  static const float w_motivation = 0.4f;
  static const float w_agency = 0.6f;

  // clang-format off
  solution->core.friction =
       w_motivation * compound_persona_motivation_get_friction(&solution->compounds.motivation)
     + w_agency * compound_persona_agency_get_friction(&solution->compounds.agency);
  // clang-format on
}

static inline void solution_persona_update_reward(SolutionPersona *solution) {

  static const float w_motivation = 0.4f;
  static const float w_agency = 0.6f;

  // clang-format off
  solution->core.reward =
       w_motivation * compound_persona_motivation_get_reward(&solution->compounds.motivation)
     + w_agency * compound_persona_agency_get_reward(&solution->compounds.agency);
  // clang-format on
}

#endif
