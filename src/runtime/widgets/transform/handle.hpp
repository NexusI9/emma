#ifndef _WIDGET_TRANSFORM_HANDLE_H_
#define _WIDGET_TRANSFORM_HANDLE_H_

#include "runtime/node/transform_handle.h"
#include <imgui/imgui.h>

namespace Widget {
namespace Transform {
class HandleShape {

public:
  HandleShape(TransformHandle *handle) : node(handle) {}
  void draw();
  ImVec2 get_p0();
  ImVec2 get_p1();

private:
  TransformHandle *node;
};

} // namespace Transform
} // namespace Widget

#endif
