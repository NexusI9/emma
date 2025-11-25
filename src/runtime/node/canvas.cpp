#include "canvas.h"

#include "runtime/geometry/boundbox.h"
#include "runtime/geometry/vector.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/module.h"
#include "runtime/node/connector.h"
#include "runtime/node/connector_handle.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/widgets/canvas.hpp"
#include "utils/id.h"

static const char *octalysis_labels[OCTAGON_VERTEX_COUNT] = {
    "Epic Meaning", "Empowerment", "Social Influence", "Unpredictability",
    "Avoidance",    "Scarcity",    "Ownership",        "Accomplishment",
};

typedef enum {
  CanvasFrameCreateFlags_None = 0,
  CanvasFrameCreateFlags_CreateOctagon = 1 << 0,
  CanvasFrameCreateFlags_CreateConnectorHandle = 1 << 1,
  CanvasFrameCreateFlags_All = ~0,
} CanvasFrameCreateFlags;

static const float octagon_base_scale = 100.0f;

static inline void canvas_get_closest_connector_handles(const Frame *,
                                                        const Frame *,
                                                        ConnectorHandle **,
                                                        ConnectorHandle **,
                                                        const int);

static inline Frame *canvas_create_frame_core(Canvas *, const FrameDescriptor *,
                                              alloc_id *, const size_t,
                                              size_t *,
                                              const CanvasFrameCreateFlags);

Frame *canvas_create_frame_core(Canvas *canvas,
                                const FrameDescriptor *frame_desc,
                                alloc_id *list_entries, const size_t capacity,
                                size_t *list_length,
                                const CanvasFrameCreateFlags flags) {

  if (canvas->frames[CanvasFrameState_Default].length == capacity)
    return NULL;

  Frame *frame = new_frame();

  if (!frame) // ERRHANDLE
    return NULL;

  {
    // === Frame setup ===
    allocator_id_list_push(list_entries, capacity, list_length, frame->id);
    frame_create(frame, frame_desc);
  }

  if (flags & CanvasFrameCreateFlags_CreateOctagon) {
    // === Octagon ===
    Octagon *oct = canvas_create_octagon(canvas);
    octagon_update_vertices(oct);

    octagon_set_labels(oct, octalysis_labels);
    octagon_update_labels_coordinates(oct);

    // Link octagon to frame
    frame_set_octagon_id(frame, oct->id);
    canvas_align_octagon_to_frame(canvas, frame);
  }

  if (flags & CanvasFrameCreateFlags_CreateConnectorHandle) {
    // === Connector Handles ===
    for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++) {
      ConnectorHandle *handle = canvas_create_connector_handle(canvas);
      frame_set_connector_handle_id(frame, (ConnectorHandleType)i, handle->id);
    }

    canvas_align_connector_handle_group_to_frame(canvas, frame);
  }
  return frame;
}

Frame *canvas_create_frame(Canvas *canvas) {

  FrameDescriptor frame_desc = {
      .background = {1.0f, 1.0f, 1.0f, 1.0f},
      .position = {200, 200},
      .size = {360, 700},
  };

  return canvas_create_frame_core(
      canvas, &frame_desc, canvas->frames[CanvasFrameState_Default].entries,
      allocator_frame_capacity(),
      &canvas->frames[CanvasFrameState_Default].length,
      CanvasFrameCreateFlags_All);
}

Frame *canvas_create_module(Canvas *canvas, const ModuleType module) {

  const ModuleDescriptor *module_desc = get_module(module);

  FrameDescriptor frame_desc = {
      .uv0 = {module_desc->uv0[0], module_desc->uv0[1]},
      .uv1 = {module_desc->uv1[0], module_desc->uv1[1]},
      .size = {module_desc->size[0], module_desc->size[1]},
      .label = module_desc->label,
  };

  return canvas_create_frame_core(
      canvas, &frame_desc, canvas->modules[CanvasModuleState_Default].entries,
      allocator_frame_capacity(),
      &canvas->modules[CanvasModuleState_Default].length,
      CanvasFrameCreateFlags_CreateConnectorHandle);
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
      .color = {0.6f, 0.6f, 0.6f, 1.0f},
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

  const float *frame_position = frame_get_world_position(frame);
  const float *frame_end_point = frame_get_end_point(frame);

  vec2 top_edge;
  glm_vec2_copy((float *)frame_end_point, top_edge);

  top_edge[1] = frame_position[1]; // align with top edge

  vec2 oct_position;
  vec2_avg_2(frame_position, top_edge, oct_position);

  glm_vec2_sub(oct_position, (vec2){0, octagon_base_scale + gap}, oct_position);

  Octagon *octagon = allocator_octagon_entry(frame->octagon_id);
  octagon_set_position(octagon, oct_position);
}

void canvas_align_connector_handle_group_to_frame(Canvas *canvas,
                                                  const Frame *frame) {

  static const int gap = 40;
  static const vec2 gaps[] = {
      {0.0f, -1.0f * gap}, // top
      {1.0f * gap, 0.0f},  // right
      {0.0f, 1.0f * gap},  // bottom
      {-1.0f * gap, 0.0f}, // left
  };

  boundbox_edges edges;
  boundbox_edges_from_points(frame->local_position, frame->end_point, edges);

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
  frame_set_local_position(frame, value);
  frame_update_world_position(frame);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

void canvas_set_frame_size(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

void canvas_frame_wrap(Canvas *canvas, Frame *frame) {
  frame_wrap(frame);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

void canvas_set_module_position(Canvas *canvas, Frame *frame,
                                const vec2 value) {
  frame_set_local_position(frame, value);
  frame_update_world_position(frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

void canvas_set_module_size(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

StaticListStatus canvas_register_frame_state(Canvas *canvas, const Frame *frame,
                                             const CanvasFrameState state) {

  return allocator_id_list_push(canvas->frames[state].entries,
                                allocator_frame_capacity(),
                                &canvas->frames[state].length, frame->id);
}

StaticListStatus canvas_unregister_frame_state(Canvas *canvas,
                                               const Frame *frame,
                                               const CanvasFrameState state) {

  return allocator_id_list_pop(canvas->frames[state].entries,
                               &canvas->frames[state].length, frame->id);
}

StaticListStatus canvas_empty_frame_state(Canvas *canvas,
                                          const CanvasFrameState state) {
  return stli_empty(canvas->frames[state].entries,
                    &canvas->frames[state].length, sizeof(alloc_id),
                    "Canvas Frame State list");
}

void canvas_get_closest_connector_handles(const Frame *frame_a,
                                          const Frame *frame_b,
                                          ConnectorHandle **closest_handle_a,
                                          ConnectorHandle **closest_handle_b,
                                          const int side) {

  float closest_distance = FLT_MAX;
  for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++) {

    // skipping unwanted sides
    if ((__builtin_ctz(side) & i) == 0)
      continue;

    ConnectorHandle *handle_a =
        allocator_connector_handle_entry(frame_a->connector_handle_id[i]);

    for (uint8_t j = 0; j < CONNECTOR_HANDLE_COUNT; j++) {

      // skipping unwanted sides
      if ((__builtin_ctz(side) & j) == 0)
        continue;

      ConnectorHandle *handle_b =
          allocator_connector_handle_entry(frame_b->connector_handle_id[j]);

      float distance =
          glm_vec2_distance(handle_a->position, handle_b->position);

      if (distance < closest_distance) {
        closest_distance = distance;
        *closest_handle_a = handle_a;
        *closest_handle_b = handle_b;
      }
    }
  }
}

void canvas_connect_frames(Canvas *canvas, Frame *frame_a, Frame *frame_b) {

  Connector *connector = new_connector();
  if (!connector) // ERRHANDLE
    return;

  // make sure it's not connected yet
  for (size_t i = 0; i < frame_a->connectors_id.length; i++) {
    for (size_t j = 0; j < frame_b->connectors_id.length; j++) {
      if (frame_a->connectors_id.entries[i] ==
          frame_b->connectors_id.entries[j])
        return;
    }
  }

  // retrieve the closest handle position
  ConnectorHandle *closest_handle_a, *closest_handle_b;
  canvas_get_closest_connector_handles(
      frame_a, frame_b, &closest_handle_a, &closest_handle_b,
      ConnectorHandleSide_Left | ConnectorHandleSide_Right);

  // create connector
  ConnectorDescriptor cn_desc = {
      .color = {0.6f, 0.6f, 0.6f, 1.0f},
      .thickness = 6.0f,
      .start = closest_handle_a,
      .end = closest_handle_b,
  };
  connector_create(connector, &cn_desc);

  allocator_id_list_push(canvas->connectors.entries,
                         allocator_connector_capacity(),
                         &canvas->connectors.length, connector->id);

  frame_register_connector(frame_a, connector->id);
  frame_register_connector(frame_b, connector->id);
}

void canvas_disconnect_frames(Canvas *canvas, const Frame *frame_a,
                              const Frame *frame_b) {}

void canvas_update_frame_connectors(Canvas *canvas, const Frame *frame) {

  for (size_t i = 0; i < frame->connectors_id.length; i++) {
    Connector *connector =
        allocator_connector_entry(frame->connectors_id.entries[i]);

    connector_compute_corners(connector);
  }
}

CanvasStatus canvas_add_module_to_frame(Canvas *canvas, Frame *frame,
                                        const ModuleType type,
                                        const vec2 position) {

  Frame *module = canvas_create_module(canvas, type);

  if (!module) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  if (frame_add_child(frame, module->id) !=
      StaticListStatus_Success) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  canvas_set_module_position(canvas, module, position);

  return CanvasStatus_Success;
}
