#ifndef _BOUNDBOX_H_
#define _BOUNDBOX_H_

#include "runtime/geometry/core.h"
#include "nkengine/include/utils.h"
#include <cglm/cglm.h>

typedef enum {
  BoundBoxEdge_Top,
  BoundBoxEdge_Right,
  BoundBoxEdge_Bottom,
  BoundBoxEdge_Left,
} BoundBoxEdge;

typedef RectCoordinate boundbox_edges[4];

EXTERN_C_BEGIN

void boundbox_edges_from_points(const vec2, const vec2, boundbox_edges);

static inline void boundbox_top_from_points(const vec2 p0, const vec2 p1,
                                            RectCoordinate *edge) {

  glm_vec2_copy((float *)p0, edge->p0);
  glm_vec2_copy((vec2){p1[0], p0[1]}, edge->p1);
}

static inline void boundbox_right_from_points(const vec2 p0, const vec2 p1,
                                              RectCoordinate *edge) {

  glm_vec2_copy((vec2){p1[0], p0[1]}, edge->p0);
  glm_vec2_copy((float *)p1, edge->p1);
}

static inline void boundbox_bottom_from_points(const vec2 p0, const vec2 p1,
                                               RectCoordinate *edge) {

  glm_vec2_copy((vec2){p1[0], p1[1]}, edge->p0);
  glm_vec2_copy((vec2){p0[0], p1[1]}, edge->p1);
}

static inline void boundbox_left_from_points(const vec2 p0, const vec2 p1,
                                             RectCoordinate *edge) {

  glm_vec2_copy((vec2){p0[0], p1[1]}, edge->p0);
  glm_vec2_copy((float *)p0, edge->p1);
}

EXTERN_C_END

#endif
