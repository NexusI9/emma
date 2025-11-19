#ifndef _WIDGET_FRAME_H_
#define _WIDGET_FRAME_H_

#include "runtime/node/frame.h"
#include "runtime/widgets/core.hpp"

namespace Widget {

class FrameShape : public TransformWidget {

public:
  FrameShape(Frame *node) : node(node), TransformWidget() {}
  void draw();
  void set_position(void *, ImVec2) override;
  void get_position(void *, ImVec2) override;
  void set_size(void *, ImVec2) override;
  void get_size(void *, ImVec2) override;

private:
  Frame *node;
};

} // namespace Widget

#endif
