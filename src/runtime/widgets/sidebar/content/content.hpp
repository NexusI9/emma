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
};

} // namespace Content
} // namespace SideBar
} // namespace Widget

#endif
