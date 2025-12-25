#ifndef _EMMA_MOTIVATIONS_CORE_H_
#define _EMMA_MOTIVATIONS_CORE_H_

#include "runtime/solutions/module/core.h"
#include "runtime/solutions/module/formulas.h"
#include "runtime/solutions/persona/core.h"
#include "runtime/solutions/persona/formulas.h"
#include "runtime/solutions/solution.h"
#include "utils/id.h"

#include <stdio.h>

#define MOTIVATION_CORE_DRIVE(_)                                               \
  _(EpicMeaning)                                                               \
  _(Empowerment)                                                               \
  _(SocialInfluence)                                                           \
  _(Unpredictability)                                                          \
  _(Avoidance)                                                                 \
  _(Scarcity)                                                                  \
  _(Ownership)                                                                 \
  _(Accomplishment)

#define MOTIVATION_HEATMAP(_)                                                  \
  _(Excitment)                                                                 \
  _(SocialBounding)                                                            \
  _(Friction)                                                                  \
  _(Reward)

typedef enum {

#define _(Label) MotivationType_##Label,
  // Core Drive enums
  MOTIVATION_CORE_DRIVE(_)
  // Heatmap enums
  MOTIVATION_HEATMAP(_)
#undef _

      MotivationType_COUNT,
} MotivationType;

typedef struct {
  alloc_id id;
  float elements[MotivationType_COUNT];
} Motivation;

static inline float motivation_get_element(const Motivation *core,
                                           const MotivationType type) {
  return core->elements[type];
}

static inline void motivation_set_element(Motivation *core,
                                          const MotivationType type,
                                          const float value) {
  core->elements[type] = value;
}

static inline void motivation_print(const Motivation *core) {

#define _(Label)                                                               \
  printf("\t" #Label ": %f\n",                                                 \
         motivation_get_element(core, MotivationType_##Label));

  printf("Core Drive:\n");
  MOTIVATION_CORE_DRIVE(_)

  printf("Heatmap:\n");
  MOTIVATION_HEATMAP(_)

  printf("===============\n");
#undef _
}

static inline void motivation_clear_elements(Motivation *motiv) {
  memset(&motiv->elements, 0, sizeof(float) * MotivationType_COUNT);
}

#endif
