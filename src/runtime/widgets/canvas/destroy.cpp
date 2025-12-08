#include "destroy.hpp"

void Widget::CanvasDestroy::active_connector(Connector **connector) {
  if (*connector)
    canvas_destroy_connector(node, *connector);
  *connector = nullptr;
}

CanvasStatus Widget::CanvasDestroy::selected_frames() {
  return canvas_destroy_all_selected_frames(node);
}
