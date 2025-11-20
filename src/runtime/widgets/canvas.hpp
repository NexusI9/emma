#ifndef _WIDGET_CANVAS_HPP_
#define _WIDGET_CANVAS_HPP_

#include "runtime/canvas/core.h"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/transform_box.hpp"

namespace Widget {

class CanvasShape {

public:
  CanvasShape(){};
  CanvasShape(Gui *, Canvas *);

  void draw();
  void sync_shapes();

private:
  Gui *gui;
  Canvas *node;
  TransformBox transform_box;
  FrameShape frame_shapes[CANVAS_MAX_FRAMES];

  void draw_frame(FrameShape *);
};

} // namespace Widget

#endif
