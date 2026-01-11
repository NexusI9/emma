#include "toggle.hpp"
#include "resources/theme.emma.h"
#include <imgui/imgui_internal.h>

void Widget::Toggle::Component::layout() {

  sizes.radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  positions.end = sizes.boundbox;

  // center label
  ImVec2 label_size = ImGui::CalcTextSize(params->label);
  positions.label = ImVec2(sizes.boundbox.x / 2 - label_size.x / 2,
                           sizes.boundbox.y / 2 - label_size.y / 2);
}

bool Widget::Toggle::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();
  bool updated = false;

  ImGui::ItemSize(sizes.boundbox);

  const ImVec2 p1 = im_vec2_add(positions.end, origin);

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
      ImGui::IsMouseHoveringRect(origin, p1))
    *params->active = !(*params->active);

  if (*params->active) {
    dl->AddImageRounded((ImTextureRef)view, origin, p1,
                        ImVec2(components.button_gradient.region->uv0[0],
                               components.button_gradient.region->uv0[1]),
                        ImVec2(components.button_gradient.region->uv1[0],
                               components.button_gradient.region->uv1[1]),
                        ImColor(255, 255, 255, 255), sizes.radius);

  } else {
    dl->AddRect(origin, p1, colors.stroke, sizes.radius);
  }

  dl->AddText(im_vec2_add(positions.label, origin), colors.label,
              params->label);

  return updated;
}
