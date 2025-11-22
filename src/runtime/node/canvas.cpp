#include "canvas.h"

#include "runtime/allocator.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/widgets/canvas.hpp"

static const char *octalysis_labels[OCTAGON_VERTEX_COUNT] = {
    "Epic Meaning", "Empowerment", "Social Influence", "Unpredictability",
    "Avoidance",    "Scarcity",    "Ownership",        "Accomplishment",
};

static const float octagon_base_scale = 100.0f;

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

  // === Octagon Generation ===
  Octagon *oct = canvas_create_octagon(canvas);
  octagon_set_outer_offset(oct, 3, 1.0f);
  octagon_set_outer_offset(oct, 1, 0.6f);
  octagon_set_outer_offset(oct, 2, 0.2f);
  octagon_update_vertices(oct);

  octagon_set_labels(oct, octalysis_labels);
  octagon_update_labels_coordinates(oct);

  // Link octagon to frame
  frame->octagon_id = oct->id;

  canvas_align_octagon_to_frame(canvas, frame);

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
        .scale = octagon_base_scale,
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

void canvas_align_octagon_to_frame(Canvas *canvas, const Frame *frame) {

  static const float gap = 100.0f;

  vec2 frame_position, frame_end_point;

  frame_get_position(frame, frame_position);
  frame_get_end_point(frame, frame_end_point);
  frame_end_point[1] = frame_position[1]; // align with top edge

  vec2 oct_position;
  glm_vec2_add(frame_position, frame_end_point, oct_position);
  glm_vec2_scale(oct_position, 0.5f, oct_position);

  glm_vec2_sub(oct_position, (vec2){0, octagon_base_scale + gap}, oct_position);

  Octagon *octagon = allocator_octagon_entry(frame->octagon_id);
  octagon_set_position(octagon, oct_position);
}

void canvas_set_frame_position(Canvas *canvas, Frame *frame,
                               const vec2 value) {
  frame_set_position(frame, value);
  canvas_align_octagon_to_frame(canvas, frame);
}


void canvas_set_frame_size(Canvas *canvas, Frame *frame,
                               const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_octagon_to_frame(canvas, frame);
}
