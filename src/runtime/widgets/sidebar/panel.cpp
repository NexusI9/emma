#include "panel.hpp"
#include "resources/theme.emma.h"
#include "runtime/widgets/utils.hpp"

void Widget::SideBar::Panel::Component::begin() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  static const ImDrawFlags side_border_flags =
      ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomRight;

  // Transparent background
  dl->AddRectFilled(position, size,
                    emma_im_color(ThemeEmmaColor_Surface_Transparent),
                    bd_radius, side_border_flags);

  // Content panel
  dl->AddRectFilled(content_position, size,
                    emma_im_color(ThemeEmmaColor_Surface_Lower), bd_radius, 0);

  // Stroke panel
  dl->AddRect(position, size,
              emma_im_color(ThemeEmmaColor_Border_Subtlest_On_Dark), bd_radius,
              side_border_flags, bd_thickness);

  ImGui::SetCursorPos(content_inner_position);
  ImGui::BeginChild("Sidepanel Content", content_size, true);
}

void Widget::SideBar::Panel::Component::end() {

  ImGui::EndChild();
}
