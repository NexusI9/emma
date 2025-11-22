#ifndef _CONNECTOR_HANDLE_SHAPE_H_
#define _CONNECTOR_HANDLE_SHAPE_H_

#include "runtime/node/connector_handle.h"
static const int CONNECTOR_HANDLE_SHAPE_HOVER_AREA = 20;

namespace Widget {

class ConnectorHandleShape {

public:
  ConnectorHandleShape(ConnectorHandle *node) : node(node) {}
  void draw();

private:
  ConnectorHandle *node;
};

} // namespace Widget

#endif
