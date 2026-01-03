#ifndef _WIDGET_SIDEBAR_SECTION_H_
#define _WIDGET_SIDEBAR_SECTION_H_

#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/input.hpp"

namespace Widget {

namespace SideBar {

namespace EditorSection {

class Component : public Widget {

public:
  Component(Gui *gui, Input::List *list) : Widget(gui), list(list) {}
  virtual void layout(){};
  virtual void draw(){};

private:
  Input::List *list;
};

} // namespace EditorSection
} // namespace SideBar
} // namespace Widget

#endif
