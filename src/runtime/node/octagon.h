#ifndef _OCTAGON_H_
#define _OCTAGON_H_

#include "nkengine/include/utils.h"
#include "utils/id.h"
#include <cglm/cglm.h>
#include <string.h>

#define OCTAGON_VERTEX_COUNT 8
static const float OCTAGON_OUTER_ANGLE = -GLM_PI * 0.5f;
static const float OCTAGON_INNER_ANGLE = GLM_PI * (3.0f / 8.0f);

typedef enum {
  OctagonStatus_Success,
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

  vec2 label_coordinates[OCTAGON_VERTEX_COUNT];
  name_t labels[OCTAGON_VERTEX_COUNT];

  vec2 position;
  float scale;
  color inner_color, outer_color;
  float inner_radius;
} Octagon;

EXTERN_C_BEGIN

OctagonStatus octagon_create(Octagon *, const OctagonDescriptor *);

OctagonStatus octagon_update_vertices(Octagon *);
OctagonStatus octagon_update_labels_coordinates(Octagon *);

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

  octagon_update_vertices(oct);
  octagon_update_labels_coordinates(oct);

  return OctagonStatus_Success;
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

  return OctagonStatus_Success;
}

static inline OctagonStatus
octagon_set_labels(Octagon *oct, const char *labels[OCTAGON_VERTEX_COUNT]) {

  for (size_t i = 0; i < OCTAGON_VERTEX_COUNT; i++)
    name_copy(labels[i], oct->labels[i]);

  return OctagonStatus_Success;
}

static inline OctagonStatus octagon_set_position(Octagon *oct,
                                                 const vec2 value) {
  glm_vec2_copy((float *)value, oct->position);

  octagon_update_vertices(oct);
  octagon_update_labels_coordinates(oct);

  return OctagonStatus_Success;
}

static inline OctagonStatus octagon_set_scale(Octagon *oct, const float value) {
  oct->scale = value;

  octagon_update_vertices(oct);
  octagon_update_labels_coordinates(oct);
  return OctagonStatus_Success;
}

static inline OctagonStatus octagon_set_inner_radius(Octagon *oct,
                                                     const float value) {
  oct->inner_radius = value;
  return OctagonStatus_Success;
}

static inline OctagonStatus octagon_set_inner_color(Octagon *oct,
                                                    const color value) {
  glm_vec4_copy((float *)value, oct->inner_color);
  return OctagonStatus_Success;
}

static inline OctagonStatus octagon_set_outer_color(Octagon *oct,
                                                    const color value) {
  glm_vec4_copy((float *)value, oct->outer_color);
  return OctagonStatus_Success;
}

EXTERN_C_END

#endif
