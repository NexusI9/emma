#ifndef _WIDGET_CANVAS_CORE_H_
#define _WIDGET_CANVAS_CORE_H_

#include "runtime/node/canvas.h"

namespace Widget {
namespace Canvas {
class Module {

public:
  Module(Gui *gui, ::Canvas *node) : gui(gui), node(node) {}
  ::Canvas *get_node() { return node; }

protected:
  Gui *gui;
  ::Canvas *node;
};

} // namespace Canvas
} // namespace Widget

#endif
