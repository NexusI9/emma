#ifndef _WIDGET_CORE_H_
#define _WIDGET_CORE_H_

#include "nkengine/include/gui.h"
#include <imgui/imgui.h>

namespace Widget {

class Widget {

public:
  Widget(Gui *gui) : gui(gui) {}

protected:
  Gui *gui;
  ImVec2 size;
};
} // namespace Widget
#endif
