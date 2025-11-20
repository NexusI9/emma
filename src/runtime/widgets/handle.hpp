#ifndef _WIDGET_HANDLE_H_
#define _WIDGET_HANDLE_H_

#include "runtime/node/handle.h"
#include <imgui/imgui.h>

namespace Widget {

class HandleShape {

public:
  HandleShape(Handle *handle) : node(handle) {}
  void draw();
  ImVec2 get_p0();
  ImVec2 get_p1();
  
private:
  Handle *node;
};

} // namespace Widget

#endif
