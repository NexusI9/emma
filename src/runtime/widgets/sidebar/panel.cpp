#include "panel.hpp"
#include "resources/theme.emma.h"
#include "runtime/widgets/utils.hpp"

void Widget::SideBar::Panel::Component::begin(const char *label) {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  static const ImDrawFlags side_border_flags =
      ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomRight;

  // Content panel
  dl->AddRectFilled(content_position, p1,
                    emma_im_color(ThemeEmmaColor_Surface_Lower), bd_radius,
                    side_border_flags);

  // Stroke panel
  dl->AddRect(p0, p1, emma_im_color(ThemeEmmaColor_Border_Subtlest_On_Dark),
              bd_radius, side_border_flags, bd_thickness);

  ImGui::SetNextWindowPos(content_inner_position);
  ImGui::BeginChild(label, content_size);
}

void Widget::SideBar::Panel::Component::end() { ImGui::EndChild(); }
