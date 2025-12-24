#ifndef _EMMA_SOLUTION_MODULE_FORMULAS_H_
#define _EMMA_SOLUTION_MODULE_FORMULAS_H_

#include "../solution.h"

SOLUTION_FORMULAS_DEF(module)

float solution_module_get_epic_meaning(const Solution *solution) {

  return compound_module_semantic_get_epic_meaning(&solution->module.semantic);
}

float solution_module_get_empowerment(const Solution *solution) {

  static const float w_semantic = 0.8f;
  static const float w_experience = 0.2f;

  return w_semantic * compound_module_experience_get_empowerment(
                          &solution->module.experience) +
         w_experience * compound_module_semantic_get_empowerment(
                            &solution->module.semantic);
}

float solution_module_get_social_influence(const Solution *solution) {

  return compound_module_social_get_influence(&solution->module.social);
}

float solution_module_get_unpredictability(const Solution *solution) {

  float unpred = 0.0f;
  for (size_t i = 0; i < solution->module.actions.count; i++)
    unpred += compound_module_reward_get_unpredictability(
                  &solution->module.actions.entries[i].reward) *
              compound_module_action_get_role_weight(
                  &solution->module.actions.entries[i].action);

  return unpred / solution->module.actions.count;
}

float solution_module_get_avoidance(const Solution *solution) {

  float avoidance = 0.0f;
  for (size_t i = 0; i < solution->module.actions.count; i++)
    avoidance += compound_module_reward_get_avoidance(
                     &solution->module.actions.entries[i].reward) *
                 compound_module_action_get_role_weight(
                     &solution->module.actions.entries[i].action);

  return avoidance / solution->module.actions.count;
}

float solution_module_get_scarcity(const Solution *solution) {

  float scarcity = 0.0f;
  for (size_t i = 0; i < solution->module.actions.count; i++)
    scarcity += compound_module_reward_get_scarcity(
        &solution->module.actions.entries[i].reward);

  return scarcity / solution->module.actions.count;
}

float solution_module_get_ownership(const Solution *solution) {

  float ownership = 0.0f;
  for (size_t i = 0; i < solution->module.actions.count; i++)
    ownership += compound_module_reward_get_ownership(
                     &solution->module.actions.entries[i].reward) *
                 compound_module_action_get_role_weight(
                     &solution->module.actions.entries[i].action);

  return ownership / solution->module.actions.count;
}

float solution_module_get_accomplishment(const Solution *solution) {

  float accomplishment = 0.0f;
  for (size_t i = 0; i < solution->module.actions.count; i++)
    accomplishment += compound_module_reward_get_accomplishment(
        &solution->module.actions.entries[i].reward);

  return accomplishment / solution->module.actions.count;
}

float solution_module_get_excitment(const Solution *solution) {

  static const float w_reward = 0.8f;
  static const float w_social = 0.2f;
  static const float w_experience = 0.8f;

  float reward_excitment = 0.0f;
  for (size_t i = 0; i < solution->module.actions.count; i++)
    reward_excitment += compound_module_reward_get_accomplishment(
                            &solution->module.actions.entries[i].reward) *
                        compound_module_action_get_role_weight(
                            &solution->module.actions.entries[i].action);

  reward_excitment /= solution->module.actions.count;

  return w_reward * reward_excitment +
         w_social *
             compound_module_social_get_excitement(&solution->module.social) +
         w_experience *
             compound_module_experience_get_excitement(
                 &solution->module.experience, solution->module.actions.count);
}

float solution_module_get_social_bounding(const Solution *solution) {

  return compound_module_social_get_bounding(&solution->module.social);
}

float solution_module_get_friction(const Solution *solution) {

  return compound_module_experience_get_friction(
      &solution->module.experience, solution->module.actions.count);
}

float solution_module_get_reward(const Solution *solution) {

  float reward = 0.0f;
  for (size_t i = 0; i < solution->module.actions.count; i++)
    reward += compound_module_reward_get_reward(
                  &solution->module.actions.entries[i].reward) *
              compound_module_action_get_role_weight(
                  &solution->module.actions.entries[i].action);

  return reward / solution->module.actions.count;
}

#endif
