#include "toggle.hpp"
#include "resources/theme.emma.h"
#include <imgui/imgui_internal.h>

void Widget::Toggle::Component::layout() {

  radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  p1 = size;

  // center label
  ImVec2 label_size = ImGui::CalcTextSize(label);
  label_p =
      ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2);
}

void Widget::Toggle::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();
  ImGui::ItemSize(size);

  if (*active) {

    dl->AddImageRounded(
        (ImTextureRef)view, im_vec2_add(p0, origin), im_vec2_add(p1, origin),
        ImVec2(button_gradient.region->uv0[0], button_gradient.region->uv0[1]),
        ImVec2(button_gradient.region->uv1[0], button_gradient.region->uv1[1]),
        ImColor(255, 255, 255, 255), radius);

  } else {

    dl->AddRect(im_vec2_add(p0, origin), im_vec2_add(p1, origin), STROKE_COLOR,
                radius);
  }

  dl->AddText(im_vec2_add(label_p, origin), LABEL_COLOR, label);
}
