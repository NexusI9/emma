#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "nkengine/include/list.h"
#include "runtime/node/connector.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/node/transform_handle.h"
#include "utils/id.h"

// clang-format off
#define ALLOCATOR_LIST(_) \
  _(  Frame,               frame,       1024  )\
  _(  TransformHandle,     handle,      1024  )\
  _(  Connector,           connector,   1024  )\
  _(  Octagon,             octagon,     1024  )



#define ALLOCATOR_ID_LIST(Capacity) \
struct{\
  alloc_id entries[Capacity]; \
  size_t length;\
}

typedef enum{
  AllocatorStatus_Succes,
  AllocatorStatus_UndefError,
}AllocatorStatus;

EXTERN_C_BEGIN

#define _(Type, Label, Capacity) \
  Type* new_##Label();\
  Type* allocator_##Label##_entry(const alloc_id); \
  static inline size_t allocator_##Label##_capacity(){ return Capacity; }

ALLOCATOR_LIST(_);
#undef _

StaticListStatus allocator_id_list_push(alloc_id*, const size_t, size_t*, const alloc_id);
StaticListStatus allocator_id_list_pop(alloc_id*, size_t*, const alloc_id);

AllocatorStatus allocator_init();

EXTERN_C_END

// clang-format on
#endif
