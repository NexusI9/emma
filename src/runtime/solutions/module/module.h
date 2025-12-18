#ifndef _EMMA_SOLUTION_MODULE_H_
#define _EMMA_SOLUTION_MODULE_H_

#include "nkengine/include/list.h"
#include "runtime/solutions/core.h"
#include "runtime/solutions/module/compounds/action.h"
#include "runtime/solutions/module/compounds/experience.h"
#include "runtime/solutions/module/compounds/reward.h"
#include "runtime/solutions/module/compounds/semantic.h"
#include "runtime/solutions/module/compounds/social.h"

typedef struct {
  CompoundAction action;
  CompoundReward reward;
} SolutionModuleAction;

typedef struct {
  CompoundExperience experience;
  CompoundSemantic semantic;
  CompoundSocial social;
  STATIC_LIST(SolutionModuleAction, COMPOUND_ACTION_CAPACITY) actions;
} SolutionModuleCompounds;

typedef struct {
  SolutionCore core;
  SolutionModuleCompounds compounds;
} SolutionModule;

static inline void
solution_module_update_epic_meaning(SolutionModule *solution) {

  solution->core.epic_meaning =
      compound_semantic_get_epic_meaning(&solution->compounds.semantic);
}

static inline void
solution_module_update_empowerment(SolutionModule *solution) {

  static const float w_semantic = 0.8f;
  static const float w_experience = 0.2f;

  solution->core.empowerment =
      w_semantic *
          compound_experience_get_empowerment(&solution->compounds.experience) +
      w_experience *
          compound_semantic_get_empowerment(&solution->compounds.semantic);
}

static inline void
solution_module_update_social_influence(SolutionModule *solution) {
  solution->core.social_influence =
      compound_social_get_influence(&solution->compounds.social);
}

static inline void
solution_module_update_upredictability(SolutionModule *solution) {

  float unpred = 0.0f;
  for (size_t i = 0; i < solution->compounds.actions.count; i++)
    unpred += compound_reward_get_unpredictability(
                  &solution->compounds.actions.entries[i].reward) *
              compound_action_get_role_weight(
                  &solution->compounds.actions.entries[i].action);

  solution->core.unpredictability = unpred / solution->compounds.actions.count;
}

static inline void solution_module_update_avoidance(SolutionModule *solution) {

  float avoidance = 0.0f;
  for (size_t i = 0; i < solution->compounds.actions.count; i++)
    avoidance += compound_reward_get_avoidance(
                     &solution->compounds.actions.entries[i].reward) *
                 compound_action_get_role_weight(
                     &solution->compounds.actions.entries[i].action);

  solution->core.avoidance = avoidance / solution->compounds.actions.count;
}

static inline void solution_module_update_scarcity(SolutionModule *solution) {

  float scarcity = 0.0f;
  for (size_t i = 0; i < solution->compounds.actions.count; i++)
    scarcity += compound_reward_get_scarcity(
        &solution->compounds.actions.entries[i].reward);

  solution->core.scarcity = scarcity / solution->compounds.actions.count;
}

static inline void solution_module_update_ownership(SolutionModule *solution) {

  float ownership = 0.0f;
  for (size_t i = 0; i < solution->compounds.actions.count; i++)
    ownership += compound_reward_get_ownership(
                     &solution->compounds.actions.entries[i].reward) *
                 compound_action_get_role_weight(
                     &solution->compounds.actions.entries[i].action);

  solution->core.ownership = ownership / solution->compounds.actions.count;
}

static inline void
solution_module_update_accomplishment(SolutionModule *solution) {

  float accomplishment = 0.0f;
  for (size_t i = 0; i < solution->compounds.actions.count; i++)
    accomplishment += compound_reward_get_accomplishment(
        &solution->compounds.actions.entries[i].reward);

  solution->core.accomplishment =
      accomplishment / solution->compounds.actions.count;
}

static inline void solution_module_update_excitment(SolutionModule *solution) {

  static const float w_reward = 0.8f;
  static const float w_social = 0.2f;
  static const float w_experience = 0.8f;

  float reward_excitment = 0.0f;
  for (size_t i = 0; i < solution->compounds.actions.count; i++)
    reward_excitment += compound_reward_get_accomplishment(
                            &solution->compounds.actions.entries[i].reward) *
                        compound_action_get_role_weight(
                            &solution->compounds.actions.entries[i].action);

  reward_excitment /= solution->compounds.actions.count;

  solution->core.excitment =
      w_reward * reward_excitment +
      w_social * compound_social_get_excitement(&solution->compounds.social) +
      w_experience *
          compound_experience_get_excitement(&solution->compounds.experience,
                                             solution->compounds.actions.count);
}

static inline void
solution_module_update_social_bounding(SolutionModule *solution) {

  solution->core.social_bounding =
      compound_social_get_bounding(&solution->compounds.social);
}

static inline void solution_module_update_friction(SolutionModule *solution) {

  solution->core.friction = compound_experience_get_friction(
      &solution->compounds.experience, solution->compounds.actions.count);
}

static inline void solution_module_update_reward(SolutionModule *solution) {

  float reward = 0.0f;
  for (size_t i = 0; i < solution->compounds.actions.count; i++)
    reward += compound_reward_get_reward(
                  &solution->compounds.actions.entries[i].reward) *
              compound_action_get_role_weight(
                  &solution->compounds.actions.entries[i].action);

  solution->core.reward = reward / solution->compounds.actions.count;
}

#endif
