#include "tab_button.hpp"

bool Widget::SideBar::TabButton::Component::update() {

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
      ImGui::IsMouseHoveringRect(position, endpoint))
    return true;

  return false;
}

void Widget::SideBar::TabButton::Component::render(bool active) {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  static const ImDrawFlags flags =
      ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomRight;

  dl->AddRectFilled(position, endpoint, active ? bg_active_color : bg_color,
                    bd_radius, flags);

  dl->AddRect(position, endpoint, active ? bd_active_color : bd_color,
              bd_radius, flags, bd_thickness);

  icon.draw();
}

void Widget::SideBar::TabButton::Component::set_position(ImVec2 pos) {

  position = pos;
  endpoint = ImVec2(position.x + width, position.y + height);

  icon.set_position(
      ImVec2(position.x + width - icon.region->size[0] - icon_padding,
             position.y + height / 2 - icon.region->size[1] / 2),
      GuiSpriteAnchor_TopLeft);
}
