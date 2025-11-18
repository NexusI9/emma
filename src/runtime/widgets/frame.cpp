#include "frame.hpp"

#include "imgui/imgui.h"
#include "nkengine/include/gui.hpp"

void Widget::FrameShape::draw() {

  ImDrawList *dl = ImGui::GetForegroundDrawList();

  // Frame rectangle

  ImVec2 p0 = ImVec2(node->position[0], node->position[1]);
  ImVec2 p1 = ImVec2(node->position[0] + node->size[0],
                     node->position[1] + node->size[1]);

  // Draw the core frame
  dl->AddRectFilled(p0, p1, im_color(node->background), 2.0f);
}
