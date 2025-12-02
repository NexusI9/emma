#ifndef _CONNECT_SYSTEM_H_
#define _CONNECT_SYSTEM_H_

#include "runtime/node/canvas.h"
#include "runtime/node/connector.h"
#include "utils/id.h"

typedef enum {
  ConnectSystemStatus_Success,
  ConnectSystemStatus_UnfoundHandle,
  ConnectSystemStatus_InvalidFrame,
  ConnectSystemStatus_UndefError,
} ConnectSystemStatus;

EXTERN_C_BEGIN

ConnectSystemStatus
connect_system_connect_handle_to_frame(ConnectorHandle*, Connector *, alloc_id *, const size_t);

EXTERN_C_END

#endif
