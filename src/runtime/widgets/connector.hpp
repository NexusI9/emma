#ifndef _CONNECTOR_SHAPE_H_
#define _CONNECTOR_SHAPE_H_

#include "nkengine/include/gui.h"
#include "runtime/node/connector.h"
#include "runtime/node/connector_handle.h"
#include "runtime/widgets/connector_handle.hpp"

namespace Widget {

namespace Connector {

class Component {
public:
  Component(Gui *gui, ::Connector *node) : gui(gui), node(node) {}
  void draw();
  void draw_handles();
  bool clickbox_hovered();
  ::Connector *get_node() { return node; }

private:
  Gui *gui;
  ::Connector *node;
  static constexpr float arrow_dist = 10.0f;
};

} // namespace Connector
} // namespace Widget

#endif
