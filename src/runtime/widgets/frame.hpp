#ifndef _WIDGET_FRAME_H_
#define _WIDGET_FRAME_H_

#include "runtime/geometry/boundbox_frame.h"
#include "runtime/node/frame.h"
#include "runtime/widgets/core.hpp"

namespace Widget {

static const float FRAME_SHAPE_BOUNDBOX_THICKNESS = 40.0f;

class FrameShape {

public:
  FrameShape() {}
  FrameShape(Frame *node) : node(node) {}
  void draw();
  Frame *get_node() { return node; }
  BoundboxFrame boundbox;

private:
  Frame *node;
};

} // namespace Widget

#endif
