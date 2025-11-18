#include "allocator.h"
#include "utils/id.h"

#define _(Type, Label, Capacity)                                               \
  typedef struct {                                                             \
    Type entries[Capacity];                                                    \
    size_t length;                                                             \
  } AllocatorList##Type;                                                       \
                                                                               \
  static AllocatorList##Type allocator_list_##Label = {0};

ALLOCATOR_LIST(_)
#undef _

// Creator
#define _(Type, Label, Capacity)                                               \
  Type *new_##Label() {                                                        \
                                                                               \
    Type *entry = NULL;                                                        \
    alloc_id id = ID_UNDEFINED;                                                \
                                                                               \
    for (size_t i = 0; i < Capacity; i++) {                                    \
                                                                               \
      entry = &allocator_list_##Label.entries[i];                              \
                                                                               \
      if (entry->id == ID_UNDEFINED) {                                         \
        id = i;                                                                \
        entry->id = id;                                                        \
        allocator_list_##Label.length++;                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    return entry;                                                              \
  }

ALLOCATOR_LIST(_);
#undef _

// Accessor
#define _(Type, Label, Capacity)                                               \
  Type *allocator_##Label##_entry(const alloc_id id) {                         \
    if (allocator_list_##Label.entries[id].id == ID_UNDEFINED)                 \
      return NULL;                                                             \
    return &allocator_list_##Label.entries[id];                                \
  }

ALLOCATOR_LIST(_);
#undef _

/**
   Set each entries id to ID_UNDEFINED, which will allow us to detect if the
   entry is occupied or not.
 */
AllocatorStatus allocator_init() {

#define _(Type, Label, Capacity)                                               \
  for (size_t i = 0; i < Capacity; i++)                                        \
    allocator_list_##Label.entries[i].id = ID_UNDEFINED;                       \
  allocator_list_##Label.length = 0;

  ALLOCATOR_LIST(_);
#undef _

  return AllocatorStatus_Succes;
}

StaticListStatus allocator_id_list_push(alloc_id *entries,
                                        const size_t capacity, size_t *length,
                                        const alloc_id id) {

  return stli_insert(entries, capacity, length, sizeof(alloc_id), (void *)&id,
                     "Allocator List");
}

StaticListStatus allocator_id_list_pop(alloc_id *entries, size_t *length,
                                       const alloc_id id) {

  return stli_remove(entries, length, sizeof(alloc_id), (void *)&id,
                     "Allocator List");
}
