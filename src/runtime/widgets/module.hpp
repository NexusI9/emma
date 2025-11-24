#ifndef _WIDGET_MODULE_H_
#define _WIDGET_MODULE_H_

#include "runtime/node/frame.h"
namespace Widget {

class ModuleShape {

public:
  ModuleShape(Frame *node) : node(node) {}
  void draw();

private:
  Frame *node;
};

} // namespace Widget

#endif
