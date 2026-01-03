#ifndef _EMMA_SOLUTION_GLOBALS_H_
#define _EMMA_SOLUTION_GLOBALS_H_

#include "runtime/manager/module.h"
#include "runtime/node/persona.h"
#include "runtime/solutions/solution.h"

extern Solution *g_module_solutions[ModuleType_COUNT];
extern Solution *g_persona_solutions[PersonaType_COUNT];

EXTERN_C_BEGIN

void global_solutions_init();

static inline Solution *global_module_solution(const ModuleType type) {
  return g_module_solutions[type];
}

static inline Solution *global_persona_solution(const PersonaType type) {
  return g_persona_solutions[type];
}

EXTERN_C_END

#endif
