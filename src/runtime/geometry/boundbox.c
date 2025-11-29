#include "boundbox.h"
#include "runtime/geometry/core.h"
#include "runtime/manager/viewport.h"
#include <stdint.h>

void boundbox_edges_from_points(const vec2 p0, const vec2 p1,
                                BoundboxFrame edges) {

  static const struct {
    const BoundBoxEdge edge;
    void (*method)(const vec2, const vec2, RectCoordinate *);
  } edge_map[] = {
      // clang-format off
      { BoundBoxEdge_Top,     boundbox_top_from_points    },
      { BoundBoxEdge_Right,   boundbox_right_from_points  },
      { BoundBoxEdge_Bottom,  boundbox_bottom_from_points },
      { BoundBoxEdge_Left,    boundbox_left_from_points   },
      // clang-format on
  };

  static const int map_count = sizeof(edge_map) / sizeof(edge_map[0]);

  for (uint8_t i = 0; i < map_count; i++)
    edge_map[i].method(p0, p1, &edges[edge_map[i].edge]);
}

void boundbox_update(RectCoordinate *rect, const vec2 p0, const vec2 p1,
                     const float padding) {
  glm_vec2_copy((vec2){p0[0] - padding, p0[1] - padding}, rect->p0);
  glm_vec2_copy((vec2){p1[0] + padding, p1[1] + padding}, rect->p1);
}

void boundbox_frame_update(BoundboxFrame box, const vec2 start, const vec2 end,
                           const float thickness) {

  float min_x = vpx(fminf(start[0], end[0]));
  float min_y = vpy(fminf(start[1], end[1]));
  float max_x = vpx(fmaxf(start[0], end[0]));
  float max_y = vpy(fmaxf(start[1], end[1]));

  // Top
  box[0].p0[0] = min_x;
  box[0].p0[1] = min_y - thickness;
  box[0].p1[0] = max_x;
  box[0].p1[1] = min_y + thickness;

  // Bottom
  box[1].p0[0] = min_x;
  box[1].p0[1] = max_y - thickness;
  box[1].p1[0] = max_x;
  box[1].p1[1] = max_y + thickness;

  // Left
  box[2].p0[0] = min_x - thickness;
  box[2].p0[1] = min_y;
  box[2].p1[0] = min_x + thickness;
  box[2].p1[1] = max_y;

  // Right
  box[3].p0[0] = max_x - thickness;
  box[3].p0[1] = min_y;
  box[3].p1[0] = max_x + thickness;
  box[3].p1[1] = max_y;
}
