#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "nkengine/include/list.h"
#include "runtime/node/connector.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/node/transform_handle.h"
#include "runtime/node/connector_handle.h"
#include "utils/id.h"

#define ALLOCATOR_MAX_FRAMES 1024

// clang-format off
#define ALLOCATOR_LIST(_) \
  _(  Frame,               frame,                 1024  )\
  _(  TransformHandle,     transform_handle,      1024  )\
  _(  ConnectorHandle,     connector_handle,      1024  )\
  _(  Connector,           connector,             1024  )\
  _(  Octagon,             octagon,               1024  )

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

AllocatorStatus allocator_init();

EXTERN_C_END

// clang-format on
#endif
