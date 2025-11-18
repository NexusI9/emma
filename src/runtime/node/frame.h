#ifndef _FRAME_H_
#define _FRAME_H_

#include "nkengine/include/utils.h"
#include "utils/id.h"
#include <cglm/cglm.h>

typedef enum {
  FrameStatus_Success,
  FrameStatus_UndefError,
} FrameStatus;

typedef struct {
  alloc_id id;
  const char *label;
  vec2 position;
  vec2 size;
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

static inline FrameStatus frame_set_size(Frame *node,
                                                  const vec2 value) {
  glm_vec2_copy((float *)value, node->size);
  return FrameStatus_Success;
}

static inline FrameStatus frame_set_position(Frame *node,
                                                      const vec2 value) {
  glm_vec2_copy((float *)value, node->position);
  return FrameStatus_Success;
}

static inline FrameStatus frame_set_background(Frame *node,
                                                        const color value) {
  glm_vec4_copy((float *)value, node->background);
  return FrameStatus_Success;
}

EXTERN_C_END
#endif
