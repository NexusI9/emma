#include "destroy.hpp"

void Widget::Canvas::Destroy::active_connector(Connector **connector) {
  if (*connector)
    canvas_destroy_connector(node, *connector);
  *connector = nullptr;
}

CanvasStatus Widget::Canvas::Destroy::selected_frames() {
  return canvas_destroy_all_selected_frames(node);
}
