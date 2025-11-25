#ifndef _LAYOUT_CONTAINER_H_
#define _LAYOUT_CONTAINER_H_

#include "nkengine/include/renderer.h"
#include "runtime/widgets/canvas.hpp"

EXTERN_C_BEGIN

void container_draw_callback(Renderer *, void *);

EXTERN_C_END

namespace Layout {

class Container {

public:
  Container(Gui *, Canvas *);
  void draw();

private:
  Gui *gui;
  Widget::CanvasShape canvas_shape;
  Widget::ToolBarShape tool_bar;
};

} // namespace Layout

#endif
