#include "canvas.h"

#include "runtime/allocator.h"
#include "runtime/geometry/boundbox.h"
#include "runtime/geometry/vector.h"
#include "runtime/node/connector_handle.h"
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

  if (!frame) // ERRHANDLE
    return NULL;

  {
    // === Frame setup ===
    allocator_id_list_push(canvas->frames.entries, capacity,
                           &canvas->frames.length, frame->id);

    FrameDescriptor frame_desc = {
        .background = {1.0f, 1.0f, 1.0f, 1.0f},
        .position = {200, 200},
        .size = {360, 700},
    };

    frame_create(frame, &frame_desc);
  }

  {
    // === Octagon ===
    Octagon *oct = canvas_create_octagon(canvas);
    octagon_update_vertices(oct);

    octagon_set_labels(oct, octalysis_labels);
    octagon_update_labels_coordinates(oct);

    // Link octagon to frame
    frame_set_octagon_id(frame, oct->id);
    canvas_align_octagon_to_frame(canvas, frame);
  }

  {
    // === Connector Handles ===
    for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++) {
      ConnectorHandle *handle = canvas_create_connector_handle(canvas);
      frame_set_connector_handle_id(frame, (ConnectorHandleType)i, handle->id);
      canvas_align_connector_handle_group_to_frame(canvas, frame);
    }
  }

  return frame;
}

Octagon *canvas_create_octagon(Canvas *canvas) {

  const size_t capacity = allocator_octagon_capacity();

  if (canvas->octagons.length == capacity)
    return NULL;

  Octagon *oct = new_octagon();

  if (!oct) // ERRHANDLE
    return NULL;

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

  return oct;
}

ConnectorHandle *canvas_create_connector_handle(Canvas *canvas) {

  const size_t capacity = allocator_connector_handle_capacity();

  if (canvas->connector_handles.length == capacity)
    return NULL;

  ConnectorHandle *handle = new_connector_handle();

  if (!handle) // ERRHANDLE
    return NULL;

  allocator_id_list_push(canvas->connector_handles.entries, capacity,
                         &canvas->connector_handles.length, handle->id);

  ConnectorHandleDescriptor handle_desc = {
      .position = {0.0f, 0.0f},
      .scale = 10.0f,
      .color = {1.0f, 1.0f, 1.0f, 1.0f},
  };
  connector_handle_create(handle, &handle_desc);

  return handle;
}

// TODO move it to shape
void canvas_draw_callback(Renderer *renderer, void *data) {
  Widget::CanvasShape *canvas_shape = (Widget::CanvasShape *)data;
  canvas_shape->draw();
}

void canvas_align_octagon_to_frame(Canvas *canvas, const Frame *frame) {

  static const int gap = 100;

  vec2 frame_position, frame_end_point;
  frame_get_position(frame, frame_position);
  frame_get_end_point(frame, frame_end_point);
  frame_end_point[1] = frame_position[1]; // align with top edge

  vec2 oct_position;
  vec2_avg_2(frame_position, frame_end_point, oct_position);

  glm_vec2_sub(oct_position, (vec2){0, octagon_base_scale + gap}, oct_position);

  Octagon *octagon = allocator_octagon_entry(frame->octagon_id);
  octagon_set_position(octagon, oct_position);
}

void canvas_align_connector_handle_group_to_frame(Canvas *canvas,
                                                  const Frame *frame) {

  static const int gap = 100;
  static const vec2 gaps[] = {
      {0.0f, -1.0f * gap}, // top
      {1.0f * gap, 0.0f},  // right
      {0.0f, 1.0f * gap},  // bottom
      {-1.0f * gap, 0.0f}, // left
  };

  boundbox_edges edges;
  boundbox_edges_from_points(frame->position, frame->end_point, edges);

  for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++) {

    ConnectorHandle *handle =
        allocator_connector_handle_entry(frame->connector_handle_id[i]);

    vec2 pos;
    vec2_avg_2(edges[i].p0, edges[i].p1, pos);
    glm_vec2_add(pos, (float *)gaps[i], pos);

    connector_handle_set_position(handle, pos);
  }
}

void canvas_set_frame_position(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_position(frame, value);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

void canvas_set_frame_size(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}
