#ifndef _FRAME_H_
#define _FRAME_H_

#include "nkengine/include/utils.h"
#include "runtime/node/connector_handle.h"
#include "utils/id.h"
#include <cglm/cglm.h>

typedef enum {
  FrameStatus_Success,
  FrameStatus_UndefError,
} FrameStatus;

typedef struct {

  alloc_id id;
  alloc_id octagon_id;
  alloc_id connector_handle_id[CONNECTOR_HANDLE_COUNT];

  const char *label;
  vec2 position;
  vec2 size;
  vec2 end_point; // pos + size, usefull to get full area for mouse interaction
  color background;

} Frame;

typedef struct {
  const char *label;
  const vec2 position;
  const vec2 size;
  const color background;
} FrameDescriptor;

EXTERN_C_BEGIN

FrameStatus frame_create(Frame *, const FrameDescriptor *);

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

EXTERN_C_END
#endif
