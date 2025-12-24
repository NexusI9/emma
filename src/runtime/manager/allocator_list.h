#ifndef _ALLOCATOR_LIST_H_
#define _ALLOCATOR_LIST_H_

#include "nkengine/include/list.h"
#include "utils/id.h"


typedef struct {
  alloc_id *entries;
  size_t *count;
} AllocIdRefList;

EXTERN_C_BEGIN

static inline alloc_id *allocator_id_list_find(alloc_id *entries, size_t count,
                                               const alloc_id id) {

  for (size_t i = 0; i < count; i++) {
    if (entries[i] == id)
      return &entries[i];
  }

  return NULL;
}

static inline StaticListStatus allocator_id_list_push(alloc_id *entries,
                                                      const size_t capacity,
                                                      size_t *length,
                                                      const alloc_id id) {

  return stli_insert(entries, capacity, length, sizeof(alloc_id), (void *)&id,
                     "Allocator List");
}

static inline StaticListStatus
allocator_id_list_push_unique(alloc_id *entries, const size_t capacity,
                              size_t *length, const alloc_id id) {

  if (allocator_id_list_find(entries, *length, id) != NULL) {
    return StaticListStatus_DuplicateEntry;
  }

  return stli_insert(entries, capacity, length, sizeof(alloc_id), (void *)&id,
                     "Allocator List");
}

static inline StaticListStatus
allocator_id_list_pop(alloc_id *entries, size_t *length, const alloc_id id) {

  return stli_remove(entries, length, sizeof(alloc_id), (void *)&id,
                     "Allocator List");
}

static inline StaticListStatus allocator_id_list_empty(alloc_id *entries,
                                                       size_t *length) {

  return stli_empty(entries, length, sizeof(alloc_id), "Allocator List");
}

EXTERN_C_END

#endif
