#ifndef _WIDGET_FRAME_H_
#define _WIDGET_FRAME_H_

#include "runtime/node/frame.h"

namespace Widget {

class FrameShape {

public:
  FrameShape(Frame *node) : node(node) {}
  void draw();

private:
  Frame *node;
};

} // namespace Widget

#endif
