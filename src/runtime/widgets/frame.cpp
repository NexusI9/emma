#include "frame.hpp"

#include "imgui/imgui.h"
#include "nkengine/include/gui.hpp"
#include "runtime/node/frame.h"

void Widget::FrameShape::draw() {

  ImDrawList *dl = ImGui::GetForegroundDrawList();

  // Frame rectangle

  ImVec2 p0 = ImVec2(node->position[0], node->position[1]);
  ImVec2 p1 = ImVec2(node->position[0] + node->size[0],
                     node->position[1] + node->size[1]);

  // Draw the core frame
  dl->AddRectFilled(p0, p1, im_color(node->background), 2.0f);
}

void Widget::FrameShape::set_position(void *data, ImVec2 value) {
  FrameShape *shape = (FrameShape *)data;

  
}

void Widget::FrameShape::get_position(void *data, ImVec2 value) {
  FrameShape *shape = (FrameShape *)data;
}

void Widget::FrameShape::set_size(void *data, ImVec2 value) {
  FrameShape *shape = (FrameShape *)data;
}

void Widget::FrameShape::get_size(void *data, ImVec2 value) {
  FrameShape *shape = (FrameShape *)data;
}
