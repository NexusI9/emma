#include "frame.hpp"

#include "imgui/imgui.h"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/viewport.h"
#include "runtime/node/frame.h"

void Widget::FrameShape::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  // Frame rectangle
  ImVec2 p0 = ImVec2(vpx(node->world_position[0]), vpy(node->world_position[1]));
  ImVec2 p1 = ImVec2(vpx(node->world_position[0] + node->size[0]),
                     vpy(node->world_position[1] + node->size[1]));

  // Draw the core frame
  dl->AddRectFilled(p0, p1, im_color(node->background), 0.0f);
}
