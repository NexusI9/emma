#include "sidebar_button.hpp"

bool Widget::SideBar::Button::Component::update() {

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
      ImGui::IsMouseHoveringRect(position, endpoint)) {
    active = true;
    return true;
  }

  active = false;
  return false;
}

void Widget::SideBar::Button::Component::render() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  const ImDrawFlags flags =
      ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomRight;

  dl->AddRectFilled(position, endpoint, active ? bg_active_color : bg_color,
                    bd_radius, flags);
  
  dl->AddRect(position, endpoint, active ? bd_active_color : bd_color,
              bd_radius, flags, bd_thickness);
}
