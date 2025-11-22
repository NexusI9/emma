#ifndef _CONNECTOR_HANDLE_H_
#define _CONNECTOR_HANDLE_H_

#include "nkengine/include/utils.h"
#include "utils/id.h"
#include <cglm/cglm.h>

static const int CONNECTOR_HANDLE_COUNT = 4;

typedef enum {
  ConnectorHandleStatus_Success,
  ConnectorHandleStatus_UndefError,
} ConnectorHandleStatus;

typedef enum {
  ConnectorHandleType_Top,
  ConnectorHandleType_Right,
  ConnectorHandleType_Bottom,
  ConnectorHandleType_Left,
} ConnectorHandleType;

typedef struct {
  const char *label;
  alloc_id id;
  vec2 position, start, end;
  float scale;
  color color;
} ConnectorHandle;

typedef struct {
  const char *label;
  const vec2 position;
  const float scale;
  const color color;
} ConnectorHandleDescriptor;

typedef ConnectorHandle ConnectorHandleGroup[4];

EXTERN_C_BEGIN

ConnectorHandleStatus
connector_handle_create(ConnectorHandle *, const ConnectorHandleDescriptor *);

static inline ConnectorHandleStatus
connector_handle_update_points(ConnectorHandle *handle) {

  glm_vec2_sub(handle->position, (vec2){handle->scale, handle->scale},
               handle->start);
  glm_vec2_add(handle->position, (vec2){handle->scale, handle->scale},
               handle->end);

  return ConnectorHandleStatus_Success;
}

static inline ConnectorHandleStatus
connector_handle_set_position(ConnectorHandle *handle, const vec2 value) {
  glm_vec2_copy((float *)value, handle->position);
  connector_handle_update_points(handle);
  return ConnectorHandleStatus_Success;
}

static inline ConnectorHandleStatus
connector_handle_set_color(ConnectorHandle *handle, const vec4 value) {
  glm_vec4_copy((float *)value, handle->color);
  return ConnectorHandleStatus_Success;
}

static inline ConnectorHandleStatus
connector_handle_set_scale(ConnectorHandle *handle, const float value) {
  handle->scale = value;
  connector_handle_update_points(handle);
  return ConnectorHandleStatus_Success;
}

EXTERN_C_END

#endif
