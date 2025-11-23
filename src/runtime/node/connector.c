#include "connector.h"
#include "runtime/geometry/vector.h"
#include "runtime/node/connector_handle.h"

ConnectorStatus connector_create(Connector *connector,
                                 const ConnectorDescriptor *desc) {

  connector_set_color(connector, desc->color);
  connector_set_start_handle(connector, desc->start);
  connector_set_end_handle(connector, desc->end);
  connector_set_thickness(connector, desc->thickness);

  connector_compute_corners(connector);

  return ConnectorStatus_Success;
}

/**
   p0                   c00
   +---------------------+--.
                            |
                            + c01
                            |
                            |
                            x (half)
                            |
                            |
                        c10 +
                            |
                            '--+--------------------+
                              c11                   p1

 */
ConnectorStatus connector_compute_corners(Connector *connector) {

  static const int corner_size = 20;

  vec2 half, p0, p1;

  connector_handle_get_position(connector->h0, p0);
  connector_handle_get_position(connector->h1, p1);
  
  vec2_avg_2(p0, p1, half);

  glm_vec2_copy((vec2){half[0] - corner_size, p0[1]}, connector->corners[0]);

  glm_vec2_copy((vec2){half[0], p0[1] + corner_size}, connector->corners[1]);

  glm_vec2_copy((vec2){half[0], p1[1] - corner_size}, connector->corners[2]);

  glm_vec2_copy((vec2){half[0] + corner_size, p1[1]}, connector->corners[3]);

  return ConnectorStatus_Success;
}

ConnectorStatus connector_swap_direction(Connector *connector) {

  const ConnectorHandle* prev_h0 = connector->h0;
  connector_set_start_handle(connector, connector->h1);
  connector_set_end_handle(connector, prev_h0);

  return ConnectorStatus_Success;
}
