#ifndef _LAYOUT_NAV_BAR_H_
#define _LAYOUT_NAV_BAR_H_

#include "nkengine/include/gui.hpp"
#include "runtime/node/canvas.h"
#include <imgui/imgui.h>

namespace Layout {

class NavBar {

public:
  NavBar(Gui *gui, Canvas *canvas);
  void draw();

private:
  Gui *gui;
  Canvas *canvas;
  static constexpr ImVec2 margin = ImVec2(50.0, 20.0);
  static constexpr uint8_t height = 42;
  static constexpr ImColor bg_color = ImColor(52, 52, 52, 255);
  static constexpr ImColor bd_color = ImColor(74, 74, 74, 255);
  static constexpr uint8_t bd_radius = 3;
  static constexpr ImVec2 padding = ImVec2(12, 6);

  UI::Frame left_panel, right_panel;
  UI::Switch oct_switch, heat_switch;
};

} // namespace Layout

#endif
