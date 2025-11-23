#ifndef _WIDGET_CANVAS_HPP_
#define _WIDGET_CANVAS_HPP_

#include "runtime/node/canvas.h"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/transform_box.hpp"

namespace Widget {

void canvas_shape_set_frame_shape_position(void *, ImVec2);
void canvas_shape_get_frame_shape_position(void *, ImVec2 &);
void canvas_shape_set_frame_shape_size(void *, ImVec2);
void canvas_shape_get_frame_shape_size(void *, ImVec2 &);
void canvas_shape_on_frame_selection(void *);

bool canvas_shape_wheel_callback(int, const EmscriptenWheelEvent *, void *);

typedef struct {
  Canvas *canvas;
  FrameShape *frame;
} CanvasTransformFrameData;

class CanvasShape {

public:
  CanvasShape(){};
  CanvasShape(Gui *, Canvas *);

  void draw();
  void sync_shapes();
  void sync_boundboxes();

private:
  Gui *gui;
  Canvas *node;
  TransformBox transform_box;

  FrameShape frame_shapes[CANVAS_MAX_FRAMES];
  CanvasTransformFrameData transform_frame_data[CANVAS_MAX_FRAMES];

  void draw_frame(FrameShape *);
  void draw_frame_handle_connectors(Frame *, const int);
};

} // namespace Widget

#endif
