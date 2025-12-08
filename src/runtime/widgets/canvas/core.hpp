#ifndef _WIDGET_CANVAS_CORE_H_
#define _WIDGET_CANVAS_CORE_H_

#include "runtime/node/canvas.h"

namespace Widget {

class CanvasModule {

public:
  CanvasModule(Gui *gui, Canvas *node) : gui(gui), node(node) {}

protected:
  Gui *gui;
  Canvas *node;
};

} // namespace Widget

#endif
