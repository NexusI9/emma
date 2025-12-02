#include "connect_system.h"

/**
     Define to which frame handle we want to hook the
     connector depending on the opposite end relative position to
     frame.
 */
ConnectSystemStatus
connect_system_connect_handle_to_frame(ConnectorHandle *handle,
                                       Connector *connector, alloc_id *frames,
                                       const size_t frames_count) {

  // check if the handle is within a frame area and handle links accordingly
  for (size_t i = 0; i < frames_count; i++) {

    Frame *frame = allocator_frame_entry(frames[i]);

    if (boundbox_contain_point(&frame->area, handle->position)) {

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
        connector_src_handle = connector->h0;
        connector_opposite_src_handle = connector->h0;
        frame_target_handle = connector->h1;
        update_handle_callback = connector_set_end_handle;
      }

      if (!connector_opposite_handle || !update_handle_callback)
        continue;

      frame_unregister_connector(frame, connector->id);

      // If the target frame already has the opposite handle connected, then
      // cancel.
      bool target_frame_valid = true;
      for (size_t j = 0; j < CONNECTOR_HANDLE_COUNT; j++) {

        if (frame->connector_handle_id[j] ==
            connector_opposite_src_handle->id) {
          // connector opposite point already connected to one of the frame
          // handle
          target_frame_valid = false;
        }
      }

      // Compute opposite connector handle
      // X position to the target frame end point to define on which frame
      // side we shall connect the new connector.
      if (target_frame_valid) {
        ConnectorHandleSide side =
            connector_opposite_handle->position[0] < frame->end_point[0]
                ? ConnectorHandleSide_Left
                : ConnectorHandleSide_Right;

        frame_target_handle = allocator_connector_handle_entry(
            frame_get_connector_handle(frame, side));
      }

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
