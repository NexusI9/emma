#include "viewport.h"

ViewportManager g_viewport_manager = {0, .scale = 1.0f};

void viewport_update(ViewportManager *vp) {

  static const float threshold = 5.0f;

  float wheel_y = (fabs(input_wheel_y()) < threshold ? 0.0f : input_wheel_y());

  // update pan
  if (!input_key(INPUT_KEY_CMD)) {

    const float pan_x =
        (fabs(input_wheel_x()) < threshold ? 0.0f : input_wheel_x());
    const float pan_y = wheel_y;

    vp->pan[0] -= pan_x * clock_delta(&g_clock) * vp->pan_sensitivity;
    vp->pan[1] -= pan_y * clock_delta(&g_clock) * vp->pan_sensitivity;

  } else {

    float old_scale = vp->scale;
    float new_scale =
        vp->scale * (1.0f + wheel_y * vp->zoom_sensitivity);

    // Clamp scale (optional)
    new_scale = glm_clamp(new_scale, 0.1f, 10.0f);

    // Adjust pan so that the mouse position remains in place
    float mouse_x = input_mouse_x();
    float mouse_y = input_mouse_y();

    vp->pan[0] = mouse_x - (mouse_x - vp->pan[0]) * (new_scale / old_scale);
    vp->pan[0] = mouse_y - (mouse_y - vp->pan[0]) * (new_scale / old_scale);

    vp->scale = new_scale;
  }
}
