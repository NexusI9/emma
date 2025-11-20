#include "canvas.h"

#include "runtime/widgets/canvas.hpp"

Frame *canvas_create_frame(Canvas *canvas) {

  const size_t capacity = allocator_frame_capacity();

  if (canvas->frames.length == capacity)
    return NULL;

  Frame *frame = new_frame();

  if (frame) {
    allocator_id_list_push(canvas->frames.entries, capacity,
                           &canvas->frames.length, frame->id);

    FrameDescriptor frame_desc = {
        .background = {1.0f, 1.0f, 1.0f, 1.0f},
        .position = {200, 200},
        .size = {360, 700},
    };

    frame_create(frame, &frame_desc);
  }

  return frame;
}

Octagon *canvas_create_octagon(Canvas *canvas) {

  const size_t capacity = allocator_octagon_capacity();

  if (canvas->frames.length == capacity)
    return NULL;

  Octagon *oct = new_octagon();

  if (oct) {
    allocator_id_list_push(canvas->octagons.entries, capacity,
                           &canvas->octagons.length, oct->id);

    OctagonDescriptor oct_desc = {
        .inner_color = {0.1f, 0.1, 0.1f, 1.0f},
        .outer_color = {1.0f, 1.0f, 1.0f, 1.0f},
        .inner_radius = 0.6f,
        .label = "Octagon",
        .position =
            {
                context_width() / 2.0f * (float)context_dpi(),
                context_height() / 2.0f * (float)context_dpi(),
            },
        .scale = 300.0f,
    };

    octagon_create(oct, &oct_desc);
  }

  return oct;
}

// TODO move it to shape
void canvas_draw_callback(Renderer *renderer, void *data) {
  Widget::CanvasShape *canvas_shape = (Widget::CanvasShape *)data;
  canvas_shape->draw();
}
