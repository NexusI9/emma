#include "connector_handle.h"
#include "runtime/manager/allocator.h"

ConnectorHandleStatus
connector_handle_create(ConnectorHandle *handle,
                        const ConnectorHandleDescriptor *desc) {

  handle->label = desc->label;
  connector_handle_set_position(handle, desc->position);
  connector_handle_set_scale(handle, desc->scale);
  connector_handle_set_color(handle, desc->color);

  return ConnectorHandleStatus_Success;
}

ConnectorHandleStatus connector_handle_destroy(ConnectorHandle *handle) {

  // DEBUG
  printf("inner handle: %p | %llu\n", handle, handle->id);

  destroy_connector_handle(handle->id);

  return ConnectorHandleStatus_Success;
}
