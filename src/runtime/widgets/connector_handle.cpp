#include "connector_handle.hpp"

#include "nkengine/include/gui.hpp"
#include "runtime/manager/viewport.h"
#include "runtime/widgets/utils.hpp"
#include <cglm/cglm.h>
#include <imgui/imgui.h>

void Widget::ConnectorHandleShape::draw() {

  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  vec2 vpos, start, end;
  vp2(node->position, vpos);
  glm_vec2_sub(vpos, (vec2){node->scale, node->scale}, start);
  glm_vec2_add(vpos, (vec2){node->scale, node->scale}, end);

  draw_list->AddRectFilled(im_vec2(start), im_vec2(end), im_color(node->color));

}
