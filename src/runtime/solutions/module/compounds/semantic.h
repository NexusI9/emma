#ifndef _EMMA_MODULE_COMPOUND_SEMANTIC_H_
#define _EMMA_MODULE_COMPOUND_SEMANTIC_H_

#include <inttypes.h>

static const uint8_t COMPOUND_MODULE_SEMANTIC_EPIC_MEANING_MIN = 0;
static const uint8_t COMPOUND_MODULE_SEMANTIC_EPIC_MEANING_MAX = 1;

static const uint8_t COMPOUND_MODULE_SEMANTIC_EMPOWERMENT_MIN = 0;
static const uint8_t COMPOUND_MODULE_SEMANTIC_EMPOWERMENT_MAX = 1;

typedef struct {
  float epic_meaning;
  float empowerment;
} CompoundModuleSemantic;

/*
   Based on designer subjective assessment, Epic Meaning should be high if user
   feels their action fulfil greater purpose.
 */
static inline float
compound_module_semantic_get_epic_meaning(const CompoundModuleSemantic *set) {
  return (float)set->epic_meaning;
}

/*
   Based on designer subjective assessment, Empowerment should be high if it
   allows user self reflection and meaningful or strategic choices.
 */
static inline float
compound_module_semantic_get_empowerment(const CompoundModuleSemantic *set) {
  return (float)set->empowerment;
}

#endif
