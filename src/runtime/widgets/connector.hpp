#ifndef _CONNECTOR_SHAPE_H_
#define _CONNECTOR_SHAPE_H_

#include "runtime/node/connector.h"
#include "runtime/node/connector_handle.h"
#include "runtime/widgets/connector_handle.hpp"

namespace Widget {

class ConnectorShape {
public:
  ConnectorShape(Connector *node) : node(node) {}
  void draw();
  Connector *get_node() { return node; }

private:
  Connector *node;
};

} // namespace Widget

#endif
