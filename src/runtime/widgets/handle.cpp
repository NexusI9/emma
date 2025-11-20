#include "handle.hpp"

#include <imgui/imgui.h>

void Widget::HandleShape::draw() {

  ImDrawList *draw = ImGui::GetWindowDrawList();
  draw->AddRectFilled(get_p0(), get_p1(), IM_COL32(255, 255, 255, 255));
}

ImVec2 Widget::HandleShape::get_p0() {
  const float x = node->position[0];
  const float y = node->position[1];
  const float scale = node->scale;
  return ImVec2(x - 1.0f * scale, y - 1.0f * scale);
}

ImVec2 Widget::HandleShape::get_p1() {
  const float x = node->position[0];
  const float y = node->position[1];
  const float scale = node->scale;
  return ImVec2(x + 1.0f * scale, y + 1.0f * scale);
}
