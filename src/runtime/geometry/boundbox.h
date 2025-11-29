#ifndef _BOUNDBOX_H_
#define _BOUNDBOX_H_

#include "nkengine/include/utils.h"
#include "runtime/geometry/core.h"
#include <cglm/cglm.h>

typedef enum {
  BoundBoxEdge_Top,
  BoundBoxEdge_Right,
  BoundBoxEdge_Bottom,
  BoundBoxEdge_Left,
} BoundBoxEdge;

/**

  Since frames shape have selectable content insinde them, we can't define
  they're whole area as the selection trigger, as a result we need to compute an
  invisibile frame around them so they only get selected when we click within
  this area.

  +---+-----------------+---+
  | .-|-----------------|-. |
  +-;-+                 +-;-+
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  +-;-+-----------------+-;-+
  | '-|-----------------|-' |
  +---+-----------------+---+


 */

#define BOUNDBOX_FRAME_RECT_COUNT 4
typedef RectCoordinate BoundboxFrame[BOUNDBOX_FRAME_RECT_COUNT];

EXTERN_C_BEGIN

void boundbox_edges_from_points(const vec2, const vec2, BoundboxFrame);

void boundbox_frame_update(BoundboxFrame, const vec2, const vec2, const float);

void boundbox_update(RectCoordinate *, const vec2, const vec2, const float);

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

bool boundbox_collide(const RectCoordinate *, const RectCoordinate *);

EXTERN_C_END

#endif
