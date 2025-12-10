#ifndef _CONNECTOR_HANDLE_SHAPE_H_
#define _CONNECTOR_HANDLE_SHAPE_H_

#include "runtime/node/connector_handle.h"

namespace Widget {

namespace ConnectorHandle {

static const int SHAPE_HOVER_AREA = 20;

class Component {

public:
  Component(const ::ConnectorHandle *node, const ConnectorHandleSide side)
      : node(node), side(side) {}

  void draw();
  const ::ConnectorHandle *get_node() { return node; }
  bool hovered();

private:
  const ::ConnectorHandle *node;
  const ConnectorHandleSide side;
};

} // namespace ConnectorHandle
} // namespace Widget

#endif
