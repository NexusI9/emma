#ifndef _FRAME_H_
#define _FRAME_H_

#include "../geometry/boundbox.h"
#include "nkengine/include/utils.h"
#include "runtime/geometry/core.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/node/connector_handle.h"
#include "utils/id.h"
#include <cglm/cglm.h>

#define FRAME_MAX_CONNECTORS 64
#define FRAME_MAX_CHILDREN 64

static const float FRAME_BOUNDBOX_THICKNESS = 40.0f;

typedef void (*frame_boundbox_updater)(RectCoordinate *, const vec2, const vec2,
                                       const float);

typedef enum {
  FrameStatus_Success,
  FrameStatus_UndefError,
} FrameStatus;

typedef struct {

  alloc_id id, octagon_id, factor_id;
  alloc_id connector_handle_id[CONNECTOR_HANDLE_COUNT];
  ALLOCATOR_ID_LIST(FRAME_MAX_CONNECTORS) connectors_id;

  const char *label;
  vec2 local_position, world_position;
  vec2 size;
  vec2 end_point; // pos + size, usefull to get full area for mouse interaction

  struct {
    RectCoordinate entries[BOUNDBOX_FRAME_RECT_COUNT];
    size_t count;
    frame_boundbox_updater update_callback;
    float padding;
  } boundbox;

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
  const frame_boundbox_updater boundbox_update_callback;
  const float boundbox_padding;
} FrameDescriptor;

EXTERN_C_BEGIN

FrameStatus frame_create(Frame *, const FrameDescriptor *);

// Accessors
static inline const float *frame_get_size(const Frame *node) {
  return node->size;
}

static inline const float *frame_get_local_position(const Frame *node) {
  return node->local_position;
}

static inline const float *frame_get_world_position(const Frame *node) {
  return node->world_position;
}

static inline const float *frame_get_end_point(const Frame *node) {
  return node->end_point;
}

static inline const float *frame_get_background(const Frame *node) {
  return node->background;
}

// Mutators
static inline FrameStatus frame_set_size(Frame *node, const vec2 value) {

  glm_vec2_copy((float *)value, node->size);

  glm_vec2_add(node->world_position, node->size, node->end_point);

  node->boundbox.update_callback(node->boundbox.entries,
                                 frame_get_world_position(node),
                                 node->end_point, FRAME_BOUNDBOX_THICKNESS);

  return FrameStatus_Success;
}

FrameStatus frame_wrap(Frame *node);
FrameStatus frame_update_world_position(Frame *node);

static inline FrameStatus frame_set_local_position(Frame *node,
                                                   const vec2 value) {
  glm_vec2_copy((float *)value, node->local_position);
  frame_update_world_position(node);
  node->boundbox.update_callback(node->boundbox.entries,
                                 frame_get_world_position(node),
                                 node->end_point, FRAME_BOUNDBOX_THICKNESS);
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

EXTERN_C_END
#endif
