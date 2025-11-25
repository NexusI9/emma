#ifndef _WIDGET_MODULE_H_
#define _WIDGET_MODULE_H_

#include "runtime/node/frame.h"
#include <imgui/imgui.h>

namespace Widget {

class ModuleShape {

public:
  ModuleShape(Frame *node) : node(node) {}
  void draw();
  void draw_fill(ImColor);
  
private:
  Frame *node;
};

} // namespace Widget

#endif
