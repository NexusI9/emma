#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "nkengine/include/utils.h"
#include "runtime/node/connector_handle.h"
#include "utils/id.h"
#include <cglm/cglm.h>

static const int CONNECTOR_TOUCH_POINT_COUNT = 2;

typedef enum {
  ConnectorStatus_Success,
  ConnectorStatus_UndefError,
} ConnectorStatus;

typedef enum {
  ConnectorDirection_Left,
  ConnectorDirection_Right,
} ConnectorDirection;

typedef vec2 connector_corners[4];

typedef struct {
  alloc_id id;
  const ConnectorHandle *h0, *h1;
  ConnectorHandle handles[CONNECTOR_TOUCH_POINT_COUNT];
  connector_corners corners;
  float thickness;
  color color;
} Connector;

typedef struct {
  const ConnectorHandle *start, *end;
  const float thickness;
  const color color;
} ConnectorDescriptor;

typedef alloc_id connection[2];

EXTERN_C_BEGIN

ConnectorStatus connector_create(Connector *, const ConnectorDescriptor *);
ConnectorStatus connector_compute_corners(Connector *);
ConnectorStatus connector_swap_direction(Connector *);

// Mutators
static inline ConnectorStatus
connector_set_start_handle(Connector *connector,
                           const ConnectorHandle *handle) {

  connector->h0 = handle;
  connector_handle_copy(connector->h0, &connector->handles[0]);

  return ConnectorStatus_Success;
}

static inline ConnectorStatus
connector_set_end_handle(Connector *connector, const ConnectorHandle *handle) {

  connector->h1 = handle;
  connector_handle_copy(connector->h1, &connector->handles[1]);

  return ConnectorStatus_Success;
}

/**
   Sync the start and end position with the handles, useful when the frame moves
   and we need to update the connector start/end position.
 */
static inline ConnectorStatus
connector_update_handle_position(Connector *connector) {
  connector_handle_set_position(&connector->handles[0],
                                connector->h0->position);
  connector_handle_set_position(&connector->handles[1],
                                connector->h1->position);
  return ConnectorStatus_Success;
}

static inline ConnectorStatus connector_set_color(Connector *connector,
                                                  const color value) {

  glm_vec4_copy((float *)value, connector->color);
  return ConnectorStatus_Success;
}

static inline ConnectorStatus connector_set_thickness(Connector *connector,
                                                      const float value) {
  connector->thickness = value;
  return ConnectorStatus_Success;
}

// Accessors
static inline const ConnectorHandle *
connector_get_start_handle(Connector *connector) {
  return connector->h0;
}

static inline const ConnectorHandle *
connector_get_end_handle(Connector *connector) {
  return connector->h1;
}

static inline ConnectorStatus connector_get_color(Connector *connector,
                                                  color dest) {
  glm_vec4_copy(connector->color, dest);
  return ConnectorStatus_Success;
}

static inline float connector_get_thickness(Connector *connector) {
  return connector->thickness;
}

static inline const vec2 *connector_get_corners(Connector *connector) {
  return connector->corners;
}

static inline ConnectorDirection connector_get_direction(Connector *connector) {
  return (connector->handles[0].position[0] < connector->handles[1].position[0])
             ? ConnectorDirection_Right
             : ConnectorDirection_Left;
}

EXTERN_C_END

#endif
