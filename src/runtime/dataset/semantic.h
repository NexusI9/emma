#ifndef _EMMA_DATASET_SEMANTIC_H_
#define _EMMA_DATASET_SEMANTIC_H_

#include <inttypes.h>

static const uint8_t DATASET_SEMANTIC_EPIC_MEANING_MIN = 0;
static const uint8_t DATASET_SEMANTIC_EPIC_MEANING_MAX = 5;

static const uint8_t DATASET_SEMANTIC_EMPOWERMENT_MIN = 0;
static const uint8_t DATASET_SEMANTIC_EMPOWERMENT_MAX = 5;

typedef struct {
  uint8_t epic_meaning;
  uint8_t empowerment;
} DataSetSemantic;

/*
   Based on designer subjective assessment, Epic Meaning should be high if user
   feels their action fulfil greater purpose.
 */
static inline float
dataset_semantic_get_epic_meaning(const DataSetSemantic *set) {
  return (float)set->epic_meaning / DATASET_SEMANTIC_EPIC_MEANING_MAX;
}

/*
   Based on designer subjective assessment, Empowerment should be high if it
   allows user self reflection and meaningful or strategic choices.
 */
static inline float
dataset_semantic_get_empowerment(const DataSetSemantic *set) {
  return (float)set->empowerment / DATASET_SEMANTIC_EMPOWERMENT_MAX;
}

#endif
