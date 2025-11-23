#ifndef _GRID_BACKGROUND_HPP_
#define _GRID_BACKGROUND_HPP_

#include "runtime/manager/viewport.h"
#include <imgui/imgui.h>

static inline void
draw_grid_background(float spacing = 32.0f, float dotSize = 3.0f,
                     ImU32 bgColor = IM_COL32(20, 20, 20, 255),
                     ImU32 dotColor = IM_COL32(120, 120, 120, 100)) {
  ImDrawList *dl = ImGui::GetBackgroundDrawList();
  const ImGuiViewport *vp = ImGui::GetMainViewport();

  ImVec2 pos = vp->Pos;
  ImVec2 size = vp->Size;

  // Background fill
  dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor);

  //  Grid dots (squares)
  if (viewport_get_scale() > 0.6) {

    const float scale = viewport_get_scale();
    const float inv_scale = 1.0f / scale;
    const float *pan = viewport_get_pan(); // Screen-space translation (pixels)
    const float half_dot_size_screen = dotSize * 0.5f;

    // World coordinates of the top-left screen corner
    // The transformation is: world = (screen - pan) / scale
    const float world_min_x = (pos.x - pan[0]) * inv_scale;
    const float world_min_y = (pos.y - pan[1]) * inv_scale;

    // World coordinates of the bottom-right screen corner
    const float world_max_x = (pos.x + size.x - pan[0]) * inv_scale;
    const float world_max_y = (pos.y + size.y - pan[1]) * inv_scale;

    // === 3. Snap the Iteration Start to the World Grid ===
    // We must start iterating at a position that is an exact multiple of
    // 'spacing', ensuring the grid lines always align perfectly to the world
    // origin (0,0).
    const float x_start_world = floorf(world_min_x / spacing) * spacing;
    const float y_start_world = floorf(world_min_y / spacing) * spacing;

    float x_w, y_w; // Variables for World Coordinates

    // === 4. Iterate and Draw ===
    for (x_w = x_start_world; x_w <= world_max_x; x_w += spacing) {
      // Convert world coordinate back to screen coordinate: screen = (world *
      // scale) + pan
      float x_screen = (x_w * scale) + pan[0];

      // Only draw if the center of the dot is actually visible on the screen
      // area.
      if (x_screen < pos.x || x_screen > pos.x + size.x)
        continue;

      for (y_w = y_start_world; y_w <= world_max_y; y_w += spacing) {
        float y_screen = (y_w * scale) + pan[1];

        if (y_screen < pos.y || y_screen > pos.y + size.y)
          continue;

        // p0 and p1 must be screen coordinates for AddRectFilled
        ImVec2 p0(x_screen - half_dot_size_screen,
                  y_screen - half_dot_size_screen);
        ImVec2 p1(x_screen + half_dot_size_screen,
                  y_screen + half_dot_size_screen);

        dl->AddRectFilled(p0, p1, dotColor, 0.0f);
      }
    }
  }
}

#endif
