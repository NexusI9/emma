#ifndef _SOLUTION_SYSTEM_H_
#define _SOLUTION_SYSTEM_H_

#include "runtime/node/frame.h"
#include "runtime/node/motivation.h"
#include "runtime/solutions/solution.h"

float solution_system_module_persona_element(const Solution *, const Solution *,
                                             const MotivationType);

void solution_system_update_motivation(const Solution *, Motivation *);


void solution_system_update_frame(Solution *, Frame *);

#endif
