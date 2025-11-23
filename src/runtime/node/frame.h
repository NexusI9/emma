#ifndef _FRAME_H_
#define _FRAME_H_

#include "nkengine/include/utils.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/node/connector_handle.h"
#include "utils/id.h"
#include <cglm/cglm.h>

#define FRAME_MAX_CONNECTORS 64
#define FRAME_MAX_CHILDREN 64

typedef enum {
  FrameStatus_Success,
  FrameStatus_UndefError,
} FrameStatus;

typedef struct {

  alloc_id id;
  alloc_id octagon_id;
  alloc_id connector_handle_id[CONNECTOR_HANDLE_COUNT];
  ALLOCATOR_ID_LIST(FRAME_MAX_CONNECTORS) connectors_id;

  const char *label;
  vec2 position;
  vec2 size;
  vec2 end_point; // pos + size, usefull to get full area for mouse interaction

  color background;
  vec2 uv0, uv1;

  alloc_id parent;
  ALLOCATOR_ID_LIST(FRAME_MAX_CHILDREN) children;

} Frame;

typedef struct {
  const char *label;
  const vec2 position;
  const vec2 size;
  const color background;
  const vec2 uv0, uv1;
} FrameDescriptor;

EXTERN_C_BEGIN

FrameStatus frame_create(Frame *, const FrameDescriptor *);

// Mutators
static inline FrameStatus frame_set_size(Frame *node, const vec2 value) {
  glm_vec2_copy((float *)value, node->size);
  glm_vec2_add(node->position, node->size, node->end_point);
  return FrameStatus_Success;
}

static inline FrameStatus frame_set_position(Frame *node, const vec2 value) {
  glm_vec2_copy((float *)value, node->position);
  glm_vec2_add(node->position, node->size, node->end_point);
  return FrameStatus_Success;
}

static inline FrameStatus frame_set_background(Frame *node, const color value) {
  glm_vec4_copy((float *)value, node->background);
  return FrameStatus_Success;
}

static inline FrameStatus frame_set_uvs(Frame *node, const vec2 uv0,
                                        const vec2 uv1) {
  glm_vec2_copy((float *)uv0, node->uv0);
  glm_vec2_copy((float *)uv1, node->uv1);
  return FrameStatus_Success;
}

static inline FrameStatus frame_set_parent(Frame *node, const alloc_id value) {
  node->parent = value;
  return FrameStatus_Success;
}

StaticListStatus frame_add_child(Frame *node, const alloc_id id);
StaticListStatus frame_remove_child(Frame *node, const alloc_id id);

static inline FrameStatus frame_set_octagon_id(Frame *node, const alloc_id id) {
  node->octagon_id = id;
  return FrameStatus_Success;
}

static inline FrameStatus
frame_set_connector_handle_id(Frame *node, const ConnectorHandleType type,
                              const alloc_id id) {
  node->connector_handle_id[type] = id;
  return FrameStatus_Success;
}

static inline StaticListStatus frame_register_connector(Frame *node,
                                                        const alloc_id id) {

  return allocator_id_list_push(node->connectors_id.entries,
                                FRAME_MAX_CONNECTORS,
                                &node->connectors_id.length, id);
}

static inline StaticListStatus frame_unregister_connector(Frame *node,
                                                          const alloc_id id) {

  return allocator_id_list_pop(node->connectors_id.entries,
                               &node->connectors_id.length, id);
}

// Accessors
static inline FrameStatus frame_get_size(const Frame *node, vec2 dest) {
  glm_vec2_copy((float *)node->size, dest);
  return FrameStatus_Success;
}

static inline FrameStatus frame_get_position(const Frame *node, vec2 dest) {
  glm_vec2_copy((float *)node->position, dest);
  return FrameStatus_Success;
}

static inline FrameStatus frame_get_end_point(const Frame *node, vec2 dest) {
  glm_vec2_copy((float *)node->end_point, dest);
  return FrameStatus_Success;
}

static inline FrameStatus frame_get_background(const Frame *node, color dest) {
  glm_vec4_copy((float *)node->background, dest);
  return FrameStatus_Success;
}

EXTERN_C_END
#endif
