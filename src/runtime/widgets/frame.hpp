#ifndef _EMMA_WIDGET_FRAME_H_
#define _EMMA_WIDGET_FRAME_H_

#include "runtime/node/frame.h"
#include "runtime/widgets/core.hpp"

namespace Widget {

class FrameShape {

public:
  FrameShape() {}
  FrameShape(Frame *node) : node(node) {}
  void draw();
  
  Frame *get_node() { return node; }

private:
  Frame *node;
};

} // namespace Widget

#endif
