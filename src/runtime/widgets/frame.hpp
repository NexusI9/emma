#ifndef _WIDGET_FRAME_H_
#define _WIDGET_FRAME_H_

#include "runtime/geometry/boundbox_frame.h"
#include "runtime/node/frame.h"
#include "runtime/widgets/core.hpp"

void frame_shape_set_position(void *, ImVec2);
void frame_shape_get_position(void *, ImVec2 &);
void frame_shape_set_size(void *, ImVec2);
void frame_shape_get_size(void *, ImVec2 &);

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
