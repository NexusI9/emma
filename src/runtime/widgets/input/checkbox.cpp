#include "checkbox.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include <imgui/imgui_internal.h>

void Widget::Checkbox::Component::layout() {

  sizes.gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  const int outer_size = gui_scale(gui, 16), inner_size = gui_scale(gui, 12);

  sizes.outer_box = ImVec2(outer_size, outer_size);
  sizes.inner_box = ImVec2(inner_size, inner_size);

  sizes.outer_radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Small));
  sizes.inner_radius = gui_scale(gui, 2);

  positions.inner_box_start =
      ImVec2(gui_scale(gui, 2.0f), gui_scale(gui, 2.0f));
  positions.inner_box_end =
      im_vec2_add(positions.inner_box_start, sizes.inner_box);

  ImGui::SetWindowFontScale(1.2);
  const ImVec2 label_size = ImGui::CalcTextSize(params->label);
  ImGui::SetWindowFontScale(1);

  sizes.boundbox =
      ImVec2(sizes.outer_box.x + sizes.gap + label_size.x, sizes.outer_box.y);

  positions.label.x = sizes.outer_box.x + sizes.gap;
}

bool Widget::Checkbox::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();
  bool updated = false;

  ImGui::ItemSize(sizes.boundbox);

  const ImVec2 p1 = im_vec2_add(sizes.outer_box, origin);

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
      ImGui::IsMouseHoveringRect(origin, p1)) {
    *params->active = !(*params->active);
    updated = true;
  }

  dl->AddRect(origin, p1, colors.stroke, sizes.outer_radius);

  if (*params->active)
    dl->AddRectFilled(im_vec2_add(origin, positions.inner_box_start),
                      im_vec2_add(origin, positions.inner_box_end),
                      colors.background, sizes.inner_radius);

  dl->AddText(im_vec2_add(positions.label, origin), colors.label,
              params->label);

  return updated;
}
