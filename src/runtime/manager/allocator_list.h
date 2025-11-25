#ifndef _ALLOCATOR_LIST_H_
#define _ALLOCATOR_LIST_H_

#include "nkengine/include/list.h"
#include "utils/id.h"

#define ALLOCATOR_ID_LIST(Capacity)                                            \
  struct {                                                                     \
    alloc_id entries[Capacity];                                                \
    size_t length;                                                             \
  }

typedef struct {
  alloc_id *entries;
  size_t *length;
} AllocIdRefList;

EXTERN_C_BEGIN

static inline StaticListStatus allocator_id_list_push(alloc_id *entries,
                                                      const size_t capacity,
                                                      size_t *length,
                                                      const alloc_id id) {

  return stli_insert(entries, capacity, length, sizeof(alloc_id), (void *)&id,
                     "Allocator List");
}

static inline StaticListStatus
allocator_id_list_pop(alloc_id *entries, size_t *length, const alloc_id id) {

  return stli_remove(entries, length, sizeof(alloc_id), (void *)&id,
                     "Allocator List");
}

EXTERN_C_END

#endif
