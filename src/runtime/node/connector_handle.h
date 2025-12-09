#ifndef _CONNECTOR_HANDLE_H_
#define _CONNECTOR_HANDLE_H_

#include "nkengine/include/utils.h"
#include "runtime/geometry/core.h"
#include "utils/id.h"
#include <cglm/cglm.h>

typedef enum {
  ConnectorHandleStatus_Success,
  ConnectorHandleStatus_UndefError,
} ConnectorHandleStatus;

typedef enum {
  ConnectorHandleSide_None = 0,
  ConnectorHandleSide_Top = 1 << 0,
  ConnectorHandleSide_Right = 1 << 1,
  ConnectorHandleSide_Bottom = 1 << 2,
  ConnectorHandleSide_Left = 1 << 3,
  ConnectorHandleSide_All = ~0,
} ConnectorHandleSide;

/**

    start
      +------------+------------+
      |            |            |
      |            |            |
      |            |            |
      |         position        |
      +------------+------------+
      |            |            |
      |            |            |
      |            |            |
      |            |            |
      +------------+------------+
                                end

 */

typedef struct {
  const char *label;
  alloc_id id;
  vec2 position, start, end;
  float scale;
  const float *color;
} ConnectorHandle;

typedef struct {
  const char *label;
  const vec2 position;
  const float scale;
  const float *color;
} ConnectorHandleDescriptor;

typedef ConnectorHandle ConnectorHandleGroup[4];

EXTERN_C_BEGIN

ConnectorHandleStatus
connector_handle_create(ConnectorHandle *, const ConnectorHandleDescriptor *);

ConnectorHandleStatus connector_handle_destroy(ConnectorHandle *);

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
connector_handle_set_color(ConnectorHandle *handle, const float *value) {
  handle->color = value;
  return ConnectorHandleStatus_Success;
}

static inline ConnectorHandleStatus
connector_handle_set_scale(ConnectorHandle *handle, const float value) {
  handle->scale = value;
  connector_handle_update_points(handle);
  return ConnectorHandleStatus_Success;
}

static inline ConnectorHandleStatus
connector_handle_get_position(const ConnectorHandle *handle, vec2 dest) {
  glm_vec2_copy((float *)handle->position, dest);
  return ConnectorHandleStatus_Success;
}

static inline ConnectorHandleStatus
connector_handle_copy(const ConnectorHandle *src, ConnectorHandle *dst) {

  dst->label = src->label;
  dst->scale = src->scale;
  glm_vec2_copy((float *)src->position, dst->position);
  glm_vec2_copy((float *)src->start, dst->start);
  glm_vec2_copy((float *)src->end, dst->end);
  dst->color = src->color;

  return ConnectorHandleStatus_Success;
}

EXTERN_C_END

#endif
