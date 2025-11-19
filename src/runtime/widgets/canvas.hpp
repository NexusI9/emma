#ifndef _WIDGET_CANVAS_HPP_
#define _WIDGET_CANVAS_HPP_

#include "runtime/canvas/core.h"
#include "runtime/widgets/transform_box.hpp"

namespace Widget {

class CanvasShape {

public:
  CanvasShape(){};
  CanvasShape(Gui *, Canvas *);
  void draw();

private:
  Gui *gui;
  Canvas *node;
  TransformBox transform_box;
};

} // namespace Widget

#endif
