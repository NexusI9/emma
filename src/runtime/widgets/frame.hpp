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
  void draw_fill(ImColor, const float);
  void draw_texture();
  void draw_pod();

  Frame *get_node() { return node; }
  bool boundbox_hovered();
  bool clickbox_hovered();

private:
  Frame *node;
};

} // namespace Widget

#endif
