#include "boundbox.h"
#include "runtime/geometry/core.h"
#include <stdint.h>

void boundbox_edges_from_points(const vec2 p0, const vec2 p1,
                                boundbox_edges edges) {

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
