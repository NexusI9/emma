#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "utils/id.h"
#include "nkengine/include/utils.h"
#include <cglm/cglm.h>

typedef enum {
  ConnectorStatus_Success,
  ConnectorStatus_UndefError,
} ConnectorStatus;

typedef struct {

  alloc_id id;
  vec2 start;
  vec2 end;
  float thickness;
  vec4 color;

} Connector;


EXTERN_C_BEGIN


EXTERN_C_END

#endif
