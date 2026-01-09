#include "combobox.hpp"
#include "resources/theme.emma.h"
#include <imgui/imgui_internal.h>

void Widget::Combobox::Component::layout() {

  sizes.radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  sizes.gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  sizes.padding_left =
      gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large));
  sizes.padding_right = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));
  sizes.padding_bottom = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Small));
  sizes.padding_top = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Small));
  sizes.popup_width = gui_scale(gui, 220);

  positions.start = ImVec2(0, ImGui::CalcTextSize(params->label).y + sizes.gap);
  positions.end =
      ImVec2(sizes.boundbox.x,
             positions.start.y +
                 gui_scale(gui, emma_size(ThemeEmmaSize_Height_Input_Small)));

  const ImVec2 chevron_position = ImVec2(positions.end.x - sizes.padding_right,
                                         positions.start.y + gui_scale(gui, 4));

  components.chevron_up.set_position(chevron_position,
                                     GuiSpriteAnchor_TopRight);

  components.chevron_down.set_position(chevron_position,
                                       GuiSpriteAnchor_TopRight);

  positions.value =
      ImVec2(sizes.padding_left, positions.start.y +
                                     (positions.end.y - positions.start.y) / 2 -
                                     ImGui::CalcTextSize(params->label).y / 2);

  sizes.boundbox = positions.end;
}

void Widget::Combobox::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();

  dl->AddText(im_vec2_add(positions.label, origin), colors.label,
              params->label);

  const ImVec2 input_start = im_vec2_add(positions.start, origin);
  const ImVec2 input_end = im_vec2_add(positions.end, origin);

  dl->AddRectFilled(input_start, input_end, colors.background, sizes.radius);

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
      ImGui::IsMouseHoveringRect(input_start, input_end))
    ImGui::OpenPopup(id);

  if (State_Open == state)
    components.chevron_up.draw_at(origin);
  else
    components.chevron_down.draw_at(origin);

  dl->AddText(im_vec2_add(positions.value, origin), colors.value,
              params->items[*params->selected]);

  ImGui::SetNextItemWidth(sizes.popup_width);

  ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, sizes.radius);
  // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, sizes.padding_right);

  ImGui::PushStyleColor(ImGuiCol_PopupBg, (ImVec4)colors.background);
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)colors.value);

  if (ImGui::BeginPopup(id)) {

    for (size_t i = 0; i < params->count; i++)
      if (ImGui::Selectable(params->items[i]))
        *params->selected = i;

    ImGui::EndPopup();
  }

  ImGui::PopStyleVar(1);
  ImGui::PopStyleColor(2);

  ImGui::ItemSize(sizes.boundbox);
}
