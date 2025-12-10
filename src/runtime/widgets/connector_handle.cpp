#include "connector_handle.hpp"

#include "nkengine/include/gui.hpp"
#include "runtime/manager/viewport.h"
#include "runtime/widgets/utils.hpp"
#include <cglm/cglm.h>
#include <imgui/imgui.h>

void Widget::ConnectorHandle::Component::draw() {

  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  vec2 vstart, vend;
  vp2(node->start, vstart);
  vp2(node->end, vend);

  draw_list->AddRectFilled(im_vec2(vstart), im_vec2(vend),
                           im_color(node->color));
}

bool Widget::ConnectorHandle::Component::hovered() {

  vec2 vstart, vend;
  vp2(node->start, vstart);
  vp2(node->end, vend);

  return ImGui::IsMouseHoveringRect(im_vec2(vstart), im_vec2(vend));
}
