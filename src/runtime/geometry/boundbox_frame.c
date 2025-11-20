#include "boundbox_frame.h"

void boundbox_frame_update(BoundboxFrame box, const vec2 start, const vec2 end,
                           const float thickness) {
  
  float min_x = fminf(start[0], end[0]);
  float min_y = fminf(start[1], end[1]);
  float max_x = fmaxf(start[0], end[0]);
  float max_y = fmaxf(start[1], end[1]);

  // --- TOP ---
  box[0].p0[0] = min_x;
  box[0].p0[1] = min_y - thickness;
  box[0].p1[0] = max_x;
  box[0].p1[1] = min_y + thickness;

  // --- BOTTOM ---
  box[1].p0[0] = min_x;
  box[1].p0[1] = max_y - thickness;
  box[1].p1[0] = max_x;
  box[1].p1[1] = max_y + thickness;

  // --- LEFT ---
  box[2].p0[0] = min_x - thickness;
  box[2].p0[1] = min_y;
  box[2].p1[0] = min_x + thickness;
  box[2].p1[1] = max_y;

  // --- RIGHT ---
  box[3].p0[0] = max_x - thickness;
  box[3].p0[1] = min_y;
  box[3].p1[0] = max_x + thickness;
  box[3].p1[1] = max_y;
}
