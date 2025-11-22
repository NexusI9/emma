#include "connector_handle.hpp"

#include "nkengine/include/gui.hpp"
#include "runtime/widgets/utils.hpp"
#include <imgui/imgui.h>

void Widget::ConnectorHandleShape::draw() {

  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  draw_list->AddRectFilled(vp_im2(im_vec2(node->start)),
                           vp_im2(im_vec2(node->end)), im_color(node->color));
}
