#include "viewport.h"

ViewportManager g_viewport_manager = {0};

void viewport_update(ViewportManager *vp) {

  static const float threshold = 5.0f;

  vp->pan[0] -= (fabs(input_wheel_x()) < threshold ? 0.0f : input_wheel_x()) *
                clock_delta(&g_clock) * vp->sensitivity;
  vp->pan[1] -= (fabs(input_wheel_y()) < threshold ? 0.0f : input_wheel_y()) *
                clock_delta(&g_clock) * vp->sensitivity;
}
