#ifndef _EMMA_SOLUTION_MODULE_CORE_H_
#define _EMMA_SOLUTION_MODULE_CORE_H_

#include "nkengine/include/list.h"
#include "runtime/solutions/module/compounds/action.h"
#include "runtime/solutions/module/compounds/experience.h"
#include "runtime/solutions/module/compounds/reward.h"
#include "runtime/solutions/module/compounds/semantic.h"
#include "runtime/solutions/module/compounds/social.h"

typedef struct {
  CompoundModuleAction action;
  CompoundModuleReward reward;
} SolutionModuleAction;

typedef struct {
  CompoundModuleExperience experience;
  CompoundModuleSemantic semantic;
  CompoundModuleSocial social;
  STATIC_LIST(SolutionModuleAction, COMPOUND_MODULE_ACTION_CAPACITY) actions;
} SolutionModule;



#endif
