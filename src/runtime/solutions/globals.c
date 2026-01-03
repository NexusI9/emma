#include "globals.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/module.h"
#include "runtime/node/persona.h"
#include "runtime/solutions/module/config.h"
#include "runtime/solutions/persona/config.h"
#include "runtime/solutions/solution.h"
#include <stdint.h>

Solution *g_module_solutions[] = {0};
Solution *g_persona_solutions[] = {0};

/**
   Create the global shared persona and module solution
 */
void global_solutions_init() {

  for (uint8_t i = 0; i < ModuleType_COUNT; i++) {

    g_module_solutions[i] = new_solution();

    if (!g_module_solutions[i]) // ERRHANDLE
      continue;

    solution_module_init(g_module_solutions[i], &SOLUTION_MODULES[i]);
  }

  for (uint8_t i = 0; i < PersonaType_COUNT; i++) {

    g_persona_solutions[i] = new_solution();

    if (!g_persona_solutions[i]) // ERRHANDLE
      continue;

    solution_persona_init(g_persona_solutions[i], &SOLUTION_PERSONAS[i]);
  }
}
