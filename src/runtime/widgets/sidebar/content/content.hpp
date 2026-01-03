#ifndef _WIDGET_SIDEBAR_CONTENT_H_
#define _WIDGET_SIDEBAR_CONTENT_H_

#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/widgets/core.hpp"

namespace Widget {

namespace SideBar {

namespace Content {

class Component : public Widget {

public:
  Component(Gui *gui) : Widget(gui) {}

  virtual void layout(){};
  virtual void draw(){};

  void draw_header(const char *label) {

    ImGui::SetWindowFontScale(1.3);
    ImGui::Text("%s", label);
    ImGui::SetWindowFontScale(1);
    ImGui::Dummy(
        ImVec2(0, gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large))));
  }
};

} // namespace Content
} // namespace SideBar
} // namespace Widget

#endif
