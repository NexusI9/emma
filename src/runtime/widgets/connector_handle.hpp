#ifndef _CONNECTOR_HANDLE_SHAPE_H_
#define _CONNECTOR_HANDLE_SHAPE_H_

#include "runtime/node/connector_handle.h"
static const int CONNECTOR_HANDLE_SHAPE_HOVER_AREA = 20;

namespace Widget {

class ConnectorHandleShape {

public:
  ConnectorHandleShape(const ConnectorHandle *node,
                       const ConnectorHandleSide side)
  : node(node), side(side) {}
  
  void draw();
  const ConnectorHandle* get_node(){ return node; }
  bool hovered();

private:
  const ConnectorHandle *node;
  const ConnectorHandleSide side;
};

} // namespace Widget

#endif
