#ifndef _CONNECTOR_SHAPE_H_
#define _CONNECTOR_SHAPE_H_

#include "runtime/node/connector.h"

namespace Widget {

class ConnectorShape {
public:
  ConnectorShape(Connector *node) : node(node) {}
  void draw();

private:
  Connector *node;
};

} // namespace Widget

#endif
