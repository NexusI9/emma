#include "connector.h"
#include "runtime/geometry/vector.h"
#include "runtime/node/connector_handle.h"
#include <math.h>

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

  vec2 half, p0, p1;

  connector_handle_get_position(connector->h0, p0);
  connector_handle_get_position(connector->h1, p1);
  vec2_avg_2(p0, p1, half);

  const int swap_y = p0[1] < p1[1] ? 1 : -1;
  const int swap_x0 = half[0] > p0[0] ? 1 : -1;
  const int swap_x1 = half[0] < p1[0] ? 1 : -1;

  static const int max_corner_size = 20;
  
  // prevent visual bug when two points are overlapping, so we slighly shift c10
  // and c11 in opposite direction so they never overlapp.
  static const float half_shift = 0.1f;

  const float corner_size_x0 = fminf(max_corner_size, fabsf(p0[0] - half[0]));
  const float corner_size_y0 = fminf(max_corner_size, fabsf(p0[1] - half[1]));
  const float corner_size_x1 = fminf(max_corner_size, fabsf(p1[0] - half[0]));
  const float corner_size_y1 = fminf(max_corner_size, fabsf(p1[1] - half[1]));

  // c00
  glm_vec2_copy((vec2){half[0] - corner_size_x0 * swap_x0, p0[1]},
                connector->corners[0]);

  // c01
  glm_vec2_copy((vec2){half[0] - half_shift, p0[1] + corner_size_y0 * swap_y},
                connector->corners[1]);

  // c10
  glm_vec2_copy((vec2){half[0] + half_shift, p1[1] - corner_size_y1 * swap_y},
                connector->corners[2]);

  // c11
  glm_vec2_copy((vec2){half[0] + corner_size_x1 * swap_x1, p1[1]},
                connector->corners[3]);

  return ConnectorStatus_Success;
}

ConnectorStatus connector_swap_direction(Connector *connector) {

  const ConnectorHandle *prev_h0 = connector->h0;
  connector_set_start_handle(connector, connector->h1);
  connector_set_end_handle(connector, prev_h0);

  return ConnectorStatus_Success;
}
