#include "transform.h"
#include "runtime/geometry/vector.h"


void canvas_align_octagon_to_frame(Canvas *canvas, const Frame *frame) {

  static const int gap = 100;

  const float *frame_position = frame_get_world_position(frame);
  const float *frame_end_point = frame_get_end_point(frame);

  vec2 top_edge;
  glm_vec2_copy((float *)frame_end_point, top_edge);

  top_edge[1] = frame_position[1]; // align with top edge

  vec2 oct_position;
  vec2_avg_2(frame_position, top_edge, oct_position);

  glm_vec2_sub(oct_position, (vec2){0, OCTAGON_BASE_SCALE + gap}, oct_position);

  Octagon *octagon = allocator_octagon_entry(frame->octagon);
  octagon_set_position(octagon, oct_position);
}

/**
   Move the 4 connectors handles according to the frame position.
   Used when we move a frame position.
 */
void canvas_align_connector_handle_group_to_frame(Canvas *canvas,
                                                  const Frame *frame) {

  static const int gap = 40;
  static const vec2 gaps[] = {
      // clang-format off
      [ ConnectorHandleSide_Top    ] = {  0.0f        , -1.0f * gap  },
      [ ConnectorHandleSide_Right  ] = {  1.0f * gap  ,        0.0f  },
      [ ConnectorHandleSide_Bottom ] = {  0.0f        ,  1.0f * gap  },
      [ ConnectorHandleSide_Left   ] = { -1.0f * gap  ,        0.0f  },
      // clang-format on
  };

  BoundboxFrame edges;
  boundbox_edges_from_points(frame->local_position, frame->end_point, edges);

  for (uint8_t i = 0; i < FRAME_CONNECTOR_HANDLE_COUNT; i++) {

    ConnectorHandle *handle =
        allocator_connector_handle_entry(frame->connector_handle_id[i]);

    vec2 pos;
    vec2_avg_2(edges[i].p0, edges[i].p1, pos);
    glm_vec2_add(pos, (float *)gaps[(1 << i)], pos);

    connector_handle_set_position(handle, pos);
  }
}

/**
  Use the cached connectors id of the frame to update update the connector
  handles position accordingly. Used when we move a frame position.
 */
void canvas_update_connectors_handle_to_frame(Canvas *canvas,
                                              const Frame *frame) {

  for (size_t i = 0; i < frame->connectors.count; i++) {
    Connector *connector =
        allocator_connector_entry(frame->connectors.entries[i]);

    connector_update_handle_position(connector);
  }
}

void canvas_set_frame_position(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_world_position(frame, value);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}

void canvas_set_frame_size(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}

void canvas_frame_wrap(Canvas *canvas, Frame *frame) {
  frame_wrap(frame);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}

/**
   Used when we add a module to a frame
 */
void canvas_set_module_local_position(Canvas *canvas, Frame *frame,
                                      const vec2 value) {
  frame_set_local_position(frame, value); // !! Relative to parent
  frame_update_world_position(frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

/**
   Used when we transform a module on the canvas
 */
void canvas_set_module_world_position(Canvas *canvas, Frame *frame,
                                      const vec2 value) {
  frame_set_world_position(frame, value);
  frame_update_world_position(frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

void canvas_set_module_size(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

void canvas_set_pod_position(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_world_position(frame, value);
  frame_update_world_position(frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}

void canvas_set_pod_size(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}


/**
   Recompute the frame's registered connectors corners. Useful when we transform
   a frame.
 */
void canvas_update_frame_connectors(Canvas *canvas, const Frame *frame) {

  for (size_t i = 0; i < frame->connectors.count; i++) {
    Connector *connector =
        allocator_connector_entry(frame->connectors.entries[i]);

    connector_update_corners(connector);
    connector_update_clickboxes(connector, CONNECTOR_CLICKBOX_THICKNESS);
  }
}

