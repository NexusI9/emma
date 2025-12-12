#ifndef _WIDGET_SIDEBAR_PANEL_H_
#define _WIDGET_SIDEBAR_PANEL_H_

#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/widgets/core.hpp"

namespace Widget {
namespace SideBar {
namespace Panel {

class Component : public Widget {

public:
  Component(Gui *gui) : Widget(gui) {}

  void begin();
  void end();

  // clang-format off
  const float padding_left = gui_scale(gui, 60);
  const float bd_radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  const float bd_thickness = gui_scale(gui, 1.0f);
  
  const ImVec2 position = ImVec2(0, gui_scale(gui, 92));
  const ImVec2 size = ImVec2(gui_scale(gui, 442), gui_scale(gui, 776));

  const ImVec2 content_padding = ImVec2(
					gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_3)),
					gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_2))
					);

  const ImVec2 content_position = ImVec2(padding_left, position.y);
  
  const ImVec2 content_inner_position = ImVec2(
					 content_padding.x + padding_left,
					 content_padding.y + position.y
					 );
  
  const ImVec2 content_size = ImVec2(size.x - padding_left - content_padding.x, size.y - content_padding.y);
  // clang-format on

private:
};

} // namespace Panel

} // namespace SideBar

} // namespace Widget

#endif
