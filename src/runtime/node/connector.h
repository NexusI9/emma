#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "nkengine/include/utils.h"
#include "runtime/geometry/core.h"
#include "runtime/node/connector_handle.h"
#include "utils/id.h"
#include <cglm/cglm.h>

static const uint8_t CONNECTOR_HANDLE_COUNT = 2;
static const uint8_t CONNECTOR_CLICKBOX_COUNT = 5;
static const uint8_t CONNECTOR_CLICKBOX_THICKNESS = 20;

static const color CONNECTOR_COLOR = {0.6f, 0.6f, 0.6f, 1.0f};
static const float CONNECTOR_THICKNESS = 6.0f;

typedef enum {
  ConnectorStatus_Success,
  ConnectorStatus_HandleUnfound,
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
  ConnectorHandle *handles[CONNECTOR_HANDLE_COUNT];
  RectCoordinate clickboxes[CONNECTOR_CLICKBOX_COUNT];
  connector_corners corners;
  float thickness;
  color color;
} Connector;

typedef struct {
  const ConnectorHandle *start, *end;
  float thickness;
  const float *color;
} ConnectorDescriptor;

typedef alloc_id connection[2];

EXTERN_C_BEGIN

ConnectorStatus connector_create(Connector *, const ConnectorDescriptor *);
ConnectorStatus connector_destroy(Connector *);
ConnectorStatus connector_swap_direction(Connector *);
ConnectorStatus connector_update_corners(Connector *);
ConnectorStatus connector_update_clickboxes(Connector *, const float);

// Mutators
// clang-format off
static inline ConnectorStatus connector_set_start_handle(Connector *, const ConnectorHandle *);
static inline ConnectorStatus connector_set_end_handle(Connector *, const ConnectorHandle *);
static inline ConnectorStatus connector_update_handle_position(Connector *);
static inline ConnectorStatus connector_set_color(Connector *, const color);
static inline ConnectorStatus connector_set_thickness(Connector *, const float);
// clang-format on

ConnectorStatus connector_set_start_handle(Connector *connector,
                                           const ConnectorHandle *handle) {

  connector->h0 = handle;
  connector_handle_copy(connector->h0, connector->handles[0]);

  return ConnectorStatus_Success;
}

ConnectorStatus connector_set_end_handle(Connector *connector,
                                         const ConnectorHandle *handle) {

  connector->h1 = handle;
  connector_handle_copy(connector->h1, connector->handles[1]);

  return ConnectorStatus_Success;
}

/**
   Sync the start and end position with the handles, useful when the frame moves
   and we need to update the connector start/end position.
 */
ConnectorStatus connector_update_handle_position(Connector *connector) {
  connector_handle_set_position(connector->handles[0], connector->h0->position);
  connector_handle_set_position(connector->handles[1], connector->h1->position);
  return ConnectorStatus_Success;
}

ConnectorStatus connector_set_color(Connector *connector, const color value) {

  glm_vec4_copy((float *)value, connector->color);
  return ConnectorStatus_Success;
}

ConnectorStatus connector_set_thickness(Connector *connector,
                                        const float value) {
  connector->thickness = value;
  return ConnectorStatus_Success;
}

// Accessors

// clang-format off
static inline const ConnectorHandle *connector_get_start_handle(Connector *);
static inline const ConnectorHandle *connector_get_end_handle(Connector *);
static inline ConnectorStatus connector_get_color(Connector *, color);
static inline float connector_get_thickness(Connector *);
static inline const vec2 *connector_get_corners(Connector *);
static inline ConnectorDirection connector_get_direction(Connector *);
// clang-format on

const ConnectorHandle *connector_get_start_handle(Connector *connector) {
  return connector->h0;
}

const ConnectorHandle *connector_get_end_handle(Connector *connector) {
  return connector->h1;
}

ConnectorStatus connector_get_color(Connector *connector, color dest) {
  glm_vec4_copy(connector->color, dest);
  return ConnectorStatus_Success;
}

float connector_get_thickness(Connector *connector) {
  return connector->thickness;
}

const vec2 *connector_get_corners(Connector *connector) {
  return connector->corners;
}

ConnectorDirection connector_get_direction(Connector *connector) {
  return (connector->handles[0]->position[0] <
          connector->handles[1]->position[0])
             ? ConnectorDirection_Right
             : ConnectorDirection_Left;
}

EXTERN_C_END

#endif
