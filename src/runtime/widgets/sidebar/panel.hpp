#ifndef _WIDGET_SIDEBAR_PANEL_H_
#define _WIDGET_SIDEBAR_PANEL_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/widgets/core.hpp"

namespace Widget {
namespace SideBar {
namespace Panel {

class Component : public Widget {

public:
  Component(Gui *gui) : Widget(gui) {}

  void begin(const char *);
  void end();
  void set_position(ImVec2 position) {
    p0 = position;

    // Outer rect
    p1.x = p0.x + init_size.x;
    p1.y = p0.y + init_size.y;

    // Content positions
    content_position = p0;
    content_inner_position =
        ImVec2(p0.x + content_padding.x, p0.y + content_padding.y);

    // Content size (width / height!)
    content_size = ImVec2(init_size.x - content_padding.x * 2.0f,
                          init_size.y - content_padding.y * 2.0f);
  }

  void disable_border_radius() { bd_radius = 0.0f; }
  void enable_border_radius() { bd_radius = init_bd_radius; }

  // clang-format off
  const float init_bd_radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  float bd_radius = init_bd_radius;
  const float bd_thickness = gui_scale(gui, 1.0f);
  const ImVec2 init_size = gui_scale_im_vec2(gui, ImVec2(420, 690));

  const ImVec2 content_padding = ImVec2(
			gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_3)),
			gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_2))
		);
  
  ImVec2 p0;
  ImVec2 p1;
  ImVec2 content_position;
  ImVec2 content_inner_position;
  ImVec2 content_size;
  // clang-format on

private:
};

} // namespace Panel

} // namespace SideBar

} // namespace Widget

#endif
