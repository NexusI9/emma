#include "frame.hpp"

#include "imgui/imgui.h"
#include "nkengine/include/gui.hpp"
#include "runtime/geometry/boundbox_frame.h"
#include "runtime/node/frame.h"

void Widget::FrameShape::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  // Frame rectangle

  ImVec2 p0 = ImVec2(node->position[0], node->position[1]);
  ImVec2 p1 = ImVec2(node->position[0] + node->size[0],
                     node->position[1] + node->size[1]);

  // Draw the core frame
  dl->AddRectFilled(p0, p1, im_color(node->background), 2.0f);
}

void frame_shape_set_position(void *data, ImVec2 value) {
  Widget::FrameShape *shape = (Widget::FrameShape *)data;
  Frame *frame = shape->get_node();

  frame_set_position(frame, (vec2){value.x, value.y});
  boundbox_frame_update(shape->boundbox, frame->position, frame->end_point,
                        Widget::FRAME_SHAPE_BOUNDBOX_THICKNESS);
}

void frame_shape_get_position(void *data, ImVec2 &value) {
  Widget::FrameShape *shape = (Widget::FrameShape *)data;
  Frame *frame = shape->get_node();

  value = im_vec2(frame->position);
}

void frame_shape_set_size(void *data, ImVec2 value) {
  Widget::FrameShape *shape = (Widget::FrameShape *)data;
  Frame *frame = shape->get_node();

  frame_set_size(frame, (vec2){value.x, value.y});
  boundbox_frame_update(shape->boundbox, frame->position, frame->end_point,
                        Widget::FRAME_SHAPE_BOUNDBOX_THICKNESS);
}

void frame_shape_get_size(void *data, ImVec2 &value) {
  Widget::FrameShape *shape = (Widget::FrameShape *)data;
  Frame *frame = shape->get_node();

  value = im_vec2(frame->size);
}
