#ifndef _WIDGET_CORE_H_
#define _WIDGET_CORE_H_

#include "nkengine/include/gui.h"
#include "runtime/canvas/core.h"
#include <imgui/imgui.h>

namespace Widget {

class Widget {

public:
  Widget(Gui *gui) : gui(gui) {}

protected:
  Gui *gui;
};

class TransformWidget {

public:
  TransformWidget(){};
  virtual void set_position(void *, ImVec2){}
  virtual void get_position(void *, ImVec2){}
  virtual void set_size(void *, ImVec2){}
  virtual void get_size(void *, ImVec2){}
};

} // namespace Widget

#endif
