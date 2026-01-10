#include "amount.hpp"
#include "resources/theme.emma.h"
#include <imgui/imgui_internal.h>

void Widget::Amount::Component::layout() {

  sizes.radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  sizes.gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  sizes.padding_left =
      gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large));
  sizes.padding_right = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));
  sizes.padding_bottom = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Small));
  sizes.padding_top = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Small));

  positions.start = ImVec2(0, ImGui::CalcTextSize(params->label).y + sizes.gap);
  positions.end =
      ImVec2(sizes.boundbox.x,
             positions.start.y +
                 gui_scale(gui, emma_size(ThemeEmmaSize_Height_Input_Small)));

  components.chevron_up.set_position(
      ImVec2(positions.end.x - sizes.padding_right,
             positions.start.y + gui_scale(gui, 1)),
      GuiSpriteAnchor_TopRight);

  components.chevron_down.set_position(
      ImVec2(positions.end.x - sizes.padding_right,
             positions.start.y + gui_scale(gui, 15)),
      GuiSpriteAnchor_TopRight);

  sizes.boundbox = positions.end;

  positions.value_end = ImVec2(
      sizes.boundbox.x - components.chevron_down.region->size[0] - sizes.gap,
      positions.end.y);
}

bool Widget::Amount::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();
  bool updated = false;

  dl->AddText(im_vec2_add(positions.label, origin), colors.label,
              params->label);

  dl->AddRectFilled(im_vec2_add(positions.start, origin),
                    im_vec2_add(positions.end, origin), colors.background,
                    sizes.radius);

  components.chevron_up.draw_at(origin);
  components.chevron_down.draw_at(origin);

  ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,
                        ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)colors.value);

  ImGui::SetCursorScreenPos(im_vec2_add(positions.start, origin));
  ImGui::SetNextItemWidth(positions.value_end.x);
  if (ImGui::DragFloat(id, params->value, params->step, params->min,
                       params->max, format))
    updated = true;

  ImGui::PopStyleColor(3);

  ImGui::SetCursorScreenPos(origin);
  ImGui::ItemSize(sizes.boundbox);

  return updated;
}
