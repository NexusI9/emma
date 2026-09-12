#include "connection.h"
#include "runtime/manager/allocator.h"
#include "runtime/node/canvas/core.h"
#include "runtime/node/canvas/create.h"
#include "runtime/node/canvas/destroy.h"
#include "runtime/node/connector.h"
#include "runtime/node/frame.h"
#include "utils/id.h"

static inline ConnectorHandle *
canvas_get_closest_connector_handle(const Frame *, vec2, const unsigned int);

static inline CanvasStatus canvas_validate_new_connection(Canvas *, Frame *,
                                                          Frame *, Connector *);

void canvas_connection_records_init(Canvas *canvas) {

  stli_create(&canvas->connection_records.capacity,
              &canvas->connection_records.count, ALLOCATOR_FRAME_CAPACITY,
              "Connection Records");

  for (size_t i = 0; i < ALLOCATOR_FRAME_CAPACITY; i++) {
    canvas->connection_records.entries[i].frame_start = ID_UNDEFINED;
    canvas->connection_records.entries[i].frame_end = ID_UNDEFINED;
  }
}

void canvas_connection_records_insert(Canvas *canvas, alloc_id connector_id,
                                      alloc_id frame_start_id,
                                      alloc_id frame_end_id) {
  canvas->connection_records.entries[connector_id].frame_start = frame_start_id;
  canvas->connection_records.entries[connector_id].frame_end = frame_end_id;
}

void canvas_connection_records_clear(Canvas *canvas, alloc_id connector_id) {
  canvas->connection_records.entries[connector_id].frame_start = ID_UNDEFINED;
  canvas->connection_records.entries[connector_id].frame_end = ID_UNDEFINED;
}

ConnectorHandle *canvas_get_closest_connector_handle(const Frame *frame,
                                                     vec2 position,
                                                     const unsigned int side) {

  float closest_distance = FLT_MAX;
  ConnectorHandle *closest_handle = NULL;

  for (uint8_t i = 0; i < FRAME_CONNECTOR_HANDLE_COUNT; i++) {

    // skipping unwanted sides
    if ((__builtin_ctz(side) & i) == 0)
      continue;

    ConnectorHandle *handle =
        allocator_connector_handle_entry(frame->connector_handle_id[i]);

    float distance = glm_vec2_distance(handle->position, position);

    if (distance < closest_distance) {
      closest_distance = distance;
      closest_handle = handle;
    }
  }

  return closest_handle;
}

CanvasStatus canvas_connect_frames(Canvas *canvas, Frame *frame_start,
                                   Frame *frame_end, Connector *connector) {

  if (!connector)
    connector = canvas_create_connector(
        canvas, &(ConnectorDescriptor){
                    .color = emma_color(ThemeEmmaColor_Border_Subtle_On_Dark),
                    .thickness = CONNECTOR_THICKNESS,
                });

  if (!connector) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  const CanvasStatus validation =
      canvas_validate_new_connection(canvas, frame_start, frame_end, connector);

  if (validation != CanvasStatus_Success) {

    connector_update_corners(connector);
    connector_update_clickboxes(connector, CONNECTOR_CLICKBOX_THICKNESS);

    return validation;
  }

  // retrieve the closest handle position
  unsigned int connectable_sides = ConnectorHandleSide_None;
  ConnectorHandle *closest_handle_start = NULL;
  ConnectorHandle *closest_handle_end = NULL;

  if (frame_end && frame_end->type == FrameType_Pod)
    connectable_sides = ConnectorHandleSide_Right;
  else
    connectable_sides = ConnectorHandleSide_Left | ConnectorHandleSide_Right;

  vec2 center;
  if (frame_start && frame_end) {
    vec2_avg(frame_start->world_position, frame_end->world_position, center);
  }

  if (frame_start) {
    closest_handle_start = canvas_get_closest_connector_handle(
        frame_start, center, connectable_sides);
    frame_register_connector(frame_start, connector->id);
  }

  if (frame_end) {
    closest_handle_end = canvas_get_closest_connector_handle(frame_end, center,
                                                             connectable_sides);
    frame_register_connector(frame_end, connector->id);
  }

  connector_set_start_handle(connector, closest_handle_start);
  connector_set_end_handle(connector, closest_handle_end);

  connector_update_corners(connector);
  connector_update_clickboxes(connector, CONNECTOR_CLICKBOX_THICKNESS);

  canvas_connection_records_insert(canvas, connector->id,
                                   frame_start ? frame_start->id : ID_UNDEFINED,
                                   frame_end ? frame_end->id : ID_UNDEFINED);

  return CanvasStatus_Success;
}

// TODO
CanvasStatus canvas_disconnect_frames(Canvas *canvas, Frame *frame_a,
                                      Frame *frame_b) {

  // find the common connector between the two frames
  alloc_id common_connector = ID_UNDEFINED;

  for (size_t i = 0; i < frame_a->connectors.count; i++)
    for (size_t j = 0; j < frame_b->connectors.count; j++)
      if (frame_a->connectors.entries[i] == frame_b->connectors.entries[j])
        common_connector = frame_a->connectors.entries[i];

  if (common_connector == ID_UNDEFINED)
    return CanvasStatus_UnfoundResource;

  canvas_destroy_connector(canvas, allocator_connector_entry(common_connector));

  return CanvasStatus_Success;
}

/**
   DELETEME?
   Retreive the two connected frames from the connector source handle (h0*,
   h1*)
 */
CanvasStatus canvas_get_connector_frames(Canvas *canvas,
                                         const Connector *connector,
                                         Frame **frame_0, Frame **frame_1) {

  if (frame_0)
    *frame_0 = NULL;
  if (frame_1)
    *frame_1 = NULL;

  for (size_t i = 0; i < canvas->frames->count; i++) {

    Frame *frame = allocator_frame_entry(canvas->frames->entries[i]);

    for (size_t j = 0; j < FRAME_CONNECTOR_HANDLE_COUNT; j++) {

      if (connector->h0 && frame_0 && !*frame_0 &&
          connector->h0->id == frame->connector_handle_id[j])
        *frame_0 = frame;

      if (connector->h1 && frame_1 && !*frame_1 &&
          connector->h1->id == frame->connector_handle_id[j])
        *frame_1 = frame;
    }

    const bool found_0 = frame_0 && *frame_0;
    const bool found_1 = frame_1 && *frame_1;

    if ((found_0 && found_1) || (!frame_0 && found_1) || (!frame_1 && found_0))
      break;
  }

  return CanvasStatus_Success;
}

/**
   Retreive the two connected frames from the connector source handle (h0*,
   h1*) from the connection records, much faster O(1) compare to O(n) for
   linear search.
 */
const CanvasConnectionRecord *
canvas_get_connector_frames_fast(Canvas *canvas, const Connector *connector) {
  return &canvas->connection_records.entries[connector->id];
}

/*
  Everytime we connect frames together we need to validate whether the
  connection is valid or not before committing the connection, a few invalid
  case can be:
  1. connecting a frame to the same one
  2. connection a end point to a Pod which is supposed to be a start point at
  all time
  3. connecting two pods together
  4. connecting twice to the same frame
 */
CanvasStatus canvas_validate_new_connection(Canvas *canvas, Frame *frame_start,
                                            Frame *frame_end,
                                            Connector *connector) {

  /*
     === Connection a end point to a Pod ===

     Pods acts as entry points, which are supposed to be a start point at
     all time
   */

  if (frame_end && frame_end->type == FrameType_Pod)
    return CanvasStatus_InvalidConnection_PodEndPoint;

  /*
    === Connecting a frame to the same one ===

    If the target frame has both connector ends, means we try to connect to the
    same frame.

    Currently we disable same frame connection cause we don't have
    any control on the connector shape, so connecting to the same frame lead to
    weird visual result.

    However I already foresee limitation especially in case
    of feedback scheme.
  */

  if (frame_end == frame_start)
    return CanvasStatus_InvalidConnection_SameFrame;

  /*
  ===  Connecting two pods together ===

  As said above, pods can only be entry points and acts as disruptors for
  child frames. Connecting two pods together doesn't make sense in this
  context.
 */

  if ((frame_start && frame_end) &&
      (FrameType_Pod == frame_start->type && FrameType_Pod == frame_end->type))
    return CanvasStatus_InvalidConnection_LinkTwoPods;

  return CanvasStatus_Success;
}
