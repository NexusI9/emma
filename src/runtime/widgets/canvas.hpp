#ifndef _WIDGET_CANVAS_HPP_
#define _WIDGET_CANVAS_HPP_

#include "runtime/geometry/core.h"
#include "runtime/node/canvas.h"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/grid_background.hpp"
#include "runtime/widgets/tool_bar.hpp"
#include "runtime/widgets/transform_box.hpp"

namespace Widget {

void canvas_shape_set_frame_shape_position(void *, ImVec2);
void canvas_shape_get_frame_shape_position(void *, ImVec2 &);
void canvas_shape_set_frame_shape_size(void *, ImVec2);
void canvas_shape_get_frame_shape_size(void *, ImVec2 &);
void canvas_shape_on_frame_selection(void *);

typedef struct {
  Canvas *canvas;
  Frame *frame;
} CanvasTransformFrameData;

class CanvasShape {

public:
  CanvasShape(Gui *, Canvas *);

  void draw(bool);

private:
  Gui *gui;
  Canvas *node;
  TransformBox transform_box;
  GridBackground grid_background;

  CanvasTransformFrameData transform_frame_data[ALLOCATOR_MAX_FRAMES];

  void draw_frame(Frame *, const TransformBoxDraw, const uint8_t);
  void draw_frame_handle_connectors(Frame *, const int);
};

} // namespace Widget

#endif
