#ifndef _WIDGET_OCTAGON_H_
#define _WIDGET_OCTAGON_H_

#include "runtime/node/octagon.h"

namespace Widget {

class OctagonShape {

public:
  OctagonShape(Octagon *node) : node(node) {}
  void draw();

private:
  Octagon *node;
};

} // namespace Widget

#endif
