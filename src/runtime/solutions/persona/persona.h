#ifndef _EMMA_SOLUTION_MODULE_H_
#define _EMMA_SOLUTION_MODULE_H_

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

  CompoundSocial social;
  CompoundMotivation motivation;
  CompoundAgency agency;
  CompoundDensity density;

} SolutionPersonaCompounds;

typedef struct {
  SolutionCore core;
  SolutionPersonaCompounds compounds;
} SolutionPersona;



#endif
