#include "connect_system.h"
#include "runtime/manager/allocator.h"
#include "runtime/node/connector_handle.h"
#include <stdint.h>

static inline ConnectorHandleSide
connect_system_get_closest_valid_handle_side(const ConnectorHandle *,
                                             const ConnectorHandle *, Frame *,
                                             const ConnectorHandleSide);

/**
     Define to which frame handle we want to hook the
     connector depending on the opposite end relative position to
     frame.
 */
ConnectSystemStatus connect_system_connect_handle_to_frame(
    ConnectorHandle *handle, Connector *connector, alloc_id *frames,
    const size_t frames_count, const unsigned int valid_sides) {

  // check if the handle is within a frame area and handle links accordingly
  for (size_t i = 0; i < frames_count; i++) {

    Frame *frame = allocator_frame_entry(frames[i]);

    if (boundbox_contain_point(&frame->boundbox, handle->position)) {

      // Define which connector handle is active and
      // which callback to call to update its handle.
      ConnectorStatus (*update_handle_callback)(Connector *,
                                                const ConnectorHandle *) = NULL;

      const ConnectorHandle *connector_opposite_handle = NULL; // start || end
      const ConnectorHandle *connector_src_handle = NULL;      // h0 || h1
      const ConnectorHandle *connector_opposite_src_handle = NULL; // h0 || h1
      const ConnectorHandle *frame_target_handle = NULL; // h0 || h1 || new

      if (&connector->handles[0] == handle) {
        connector_opposite_handle = &connector->handles[1];
        connector_src_handle = connector->h0;
        connector_opposite_src_handle = connector->h1;
        frame_target_handle = connector->h0;
        update_handle_callback = connector_set_start_handle;
      } else if (&connector->handles[1] == handle) {
        connector_opposite_handle = &connector->handles[0];
        connector_src_handle = connector->h1;
        connector_opposite_src_handle = connector->h0;
        frame_target_handle = connector->h1;
        update_handle_callback = connector_set_end_handle;
      }

      if (!connector_opposite_handle || !update_handle_callback)
        continue;

      frame_unregister_connector(frame, connector->id);

      const ConnectorHandleSide side =
          connect_system_get_closest_valid_handle_side(
              connector_opposite_handle, connector_opposite_src_handle, frame,
              valid_sides);

      if (ConnectorHandleSide_None != side)
        frame_target_handle = allocator_connector_handle_entry(
            frame_get_connector_handle(frame, side));

      if (update_handle_callback(connector, frame_target_handle) ==
          ConnectorStatus_Success) {
        // need to register so the frame sync the
        // connector on transform
        frame_register_connector(frame, connector->id);
        connector_compute_corners(connector);

        return ConnectSystemStatus_Success;
      }
    }
  }

  return ConnectSystemStatus_UnfoundHandle;
}

/**
  If the target frame already has the opposite handle connected, then
  cancel.
  Traverse the frame's handles and get the closest one to the opposite
  connector handle. It also take in account the input sides as to only
  output the ones desired.

   Note that we don't use the active handle as reference to get the closes point
  but it's opposite (static) handle. This helps ensure to connect to the closest
  frame handle relative tothis opposite handle making sure the connector line
  are not crossing over the frame to reach the connector handle.


 */
static inline ConnectorHandleSide connect_system_get_closest_valid_handle_side(
    const ConnectorHandle *connector_opposite_handle,
    const ConnectorHandle *connector_opposite_src_handle, Frame *frame,
    const ConnectorHandleSide valid_sides) {

  ConnectorHandleSide side = ConnectorHandleSide_None;
  float min_distance = FLT_MAX;

  for (size_t j = 0; j < FRAME_CONNECTOR_HANDLE_COUNT; j++) {

    ConnectorHandle *frame_handle =
        allocator_connector_handle_entry(frame->connector_handle_id[j]);

    const float distance =
        glm_vec2_distance((float *)connector_opposite_handle->position,
                          (float *)frame_handle->position);

    if ((valid_sides & 1 << j) && distance < min_distance) {
      min_distance = distance;
      side = (1 << j);
    }

    if (frame->connector_handle_id[j] == connector_opposite_src_handle->id) {
      // connector opposite point already connected to one of the frame
      // handle
      side = ConnectorHandleSide_None;
    }
  }

  return side;
}
