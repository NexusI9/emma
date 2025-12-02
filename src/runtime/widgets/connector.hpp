#ifndef _CONNECTOR_SHAPE_H_
#define _CONNECTOR_SHAPE_H_

#include "nkengine/include/gui.h"
#include "runtime/node/connector.h"
#include "runtime/node/connector_handle.h"
#include "runtime/widgets/connector_handle.hpp"

namespace Widget {

class ConnectorShape {
public:
  ConnectorShape(Gui *gui, Connector *node) : gui(gui), node(node) {}
  void draw();
  Connector *get_node() { return node; }

private:
  Gui *gui;
  Connector *node;
  static constexpr float arrow_dist = 10.0f;
};

} // namespace Widget

#endif
