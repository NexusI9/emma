#ifndef _GRID_BACKGROUND_HPP_
#define _GRID_BACKGROUND_HPP_

#include <imgui/imgui.h>

static inline void
draw_grid_background(float spacing = 32.0f, float dotSize = 3.0f,
                     ImU32 bgColor = IM_COL32(20, 20, 20, 255),
                     ImU32 dotColor = IM_COL32(120, 120, 120, 100)) {
  ImDrawList *dl = ImGui::GetBackgroundDrawList();
  const ImGuiViewport *vp = ImGui::GetMainViewport();

  ImVec2 pos = vp->Pos;
  ImVec2 size = vp->Size;

  // --- 1) Background fill (fast, only 2 triangles) ---
  dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor);

  // --- 2) Grid dots (squares) ---
  for (float x = pos.x; x < pos.x + size.x; x += spacing) {
    for (float y = pos.y; y < pos.y + size.y; y += spacing) {
      float half = dotSize * 0.5f;

      ImVec2 p0(x - half, y - half);
      ImVec2 p1(x + half, y + half);

      dl->AddRectFilled(p0, p1, dotColor, 0.0f);
    }
  }
}

#endif
