#ifndef _WIDGET_OCTAGON_H_
#define _WIDGET_OCTAGON_H_

#include "runtime/node/octagon.h"
#include <imgui/imgui.h>

namespace Widget {

  
class OctagonShape {

public:
  OctagonShape(Octagon *node) : node(node) {}
  void draw();

private:
  Octagon *node;
  void draw_labels(ImDrawList *);
  void draw_outer_gradient(ImDrawList *);
  
  ImU32 vertex_color_rgb(const int);
  ImU32 vertex_color(const int);
};

} // namespace Widget

#endif
