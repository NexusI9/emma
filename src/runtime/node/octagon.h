#ifndef _OCTAGON_H_
#define _OCTAGON_H_

#include "nkengine/include/utils.h"
#include "utils/id.h"
#include <cglm/cglm.h>

#define OCTAGON_VERTEX_COUNT 8
static const float OCTAGON_OUTER_ANGLE = -GLM_PI * 0.5f;
static const float OCTAGON_INNER_ANGLE = GLM_PI * (3.0f / 8.0f);

typedef enum {
  OctagonStatus_Succes,
  OctagonStatus_OutOfBound,
  OctagonStatus_UndefError,
} OctagonStatus;

typedef struct {
  const char *label;
  const float scale;
  const color inner_color, outer_color;
  const float inner_radius;
  const vec2 position;
} OctagonDescriptor;

typedef struct {
  alloc_id id;
  const char *label;
  float outer_offsets[OCTAGON_VERTEX_COUNT];
  float inner_offsets[OCTAGON_VERTEX_COUNT];
  vec2 outer_vertices[OCTAGON_VERTEX_COUNT];
  vec2 inner_vertices[OCTAGON_VERTEX_COUNT];
  vec2 position;
  float scale;
  color inner_color, outer_color;
  float inner_radius;
} Octagon;

EXTERN_C_BEGIN

OctagonStatus octagon_create(Octagon *, const OctagonDescriptor *);

static inline OctagonStatus octagon_set_outer_offset(Octagon *oct,
                                                     const uint8_t vertex,
                                                     const float value) {

  if (vertex > OCTAGON_VERTEX_COUNT) {
    fprintf(stderr,
            "Attempting to assign a value to a peak superior to %d for Octagon "
            "'%s'.",
            OCTAGON_VERTEX_COUNT, oct->label);
    return OctagonStatus_OutOfBound;
  }

  oct->outer_offsets[vertex] = value;

  return OctagonStatus_Succes;
}

static inline OctagonStatus octagon_set_inner_offset(Octagon *oct,
                                                     const uint8_t vertex,
                                                     const float value) {

  if (vertex > OCTAGON_VERTEX_COUNT) {
    fprintf(stderr,
            "Attempting to assign a value to a peak superior to %d for Octagon "
            "'%s'.",
            OCTAGON_VERTEX_COUNT, oct->label);
    return OctagonStatus_OutOfBound;
  }

  oct->inner_offsets[vertex] = value;

  return OctagonStatus_Succes;
}

static inline OctagonStatus octagon_set_position(Octagon *oct,
                                                 const vec2 value) {
  glm_vec2_copy((float *)value, oct->position);
  return OctagonStatus_Succes;
}

static inline OctagonStatus octagon_set_scale(Octagon *oct, const float value) {
  oct->scale = value;
  return OctagonStatus_Succes;
}

static inline OctagonStatus octagon_set_inner_radius(Octagon *oct,
                                                     const float value) {
  oct->inner_radius = value;
  return OctagonStatus_Succes;
}

static inline OctagonStatus octagon_set_inner_color(Octagon *oct,
                                                    const color value) {
  glm_vec4_copy((float *)value, oct->inner_color);
  return OctagonStatus_Succes;
}

static inline OctagonStatus octagon_set_outer_color(Octagon *oct,
                                                    const color value) {
  glm_vec4_copy((float *)value, oct->outer_color);
  return OctagonStatus_Succes;
}

OctagonStatus octagon_update_vertices(Octagon *);

EXTERN_C_END

#endif
