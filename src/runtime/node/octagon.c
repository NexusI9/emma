#include "octagon.h"
#include <stdio.h>

OctagonStatus octagon_create(Octagon *oct, const OctagonDescriptor *desc) {

  oct->label = desc->label;

  octagon_set_scale(oct, desc->scale);
  octagon_set_outer_color(oct, desc->outer_color);
  octagon_set_inner_color(oct, desc->inner_color);
  octagon_set_inner_radius(oct, desc->inner_radius);
  octagon_set_position(oct, desc->position);

  octagon_update_vertices(oct);

  return OctagonStatus_Succes;
}

static inline OctagonStatus
octagon_update_vertices_core(vec2 *list, const float *offsets,
                             const vec2 position, const float scale,
                             const float start_angle, const float radius) {

  for (int i = 0; i < OCTAGON_VERTEX_COUNT; i++) {
    // Calculate the angle for the current vertex

    static const float denom = 2.0f * GLM_PI / (float)OCTAGON_VERTEX_COUNT;
    float angle = start_angle + (float)i * denom;

    const float scaled_radius = radius * scale;
    const float scaled_offset = offsets[i] * scale;

    // Calculate the vertex position
    glm_vec2_copy(
        (vec2){
            position[0] + (scaled_offset + scaled_radius) * cosf(angle),
            position[1] + (scaled_offset + scaled_radius) * sinf(angle),
        },
        list[i]);
  }

  return OctagonStatus_Succes;
}

OctagonStatus octagon_update_vertices(Octagon *oct) {

  octagon_update_vertices_core(oct->outer_vertices, oct->outer_offsets, oct->position,
                               oct->scale, OCTAGON_OUTER_ANGLE, 1.0f);

  octagon_update_vertices_core(oct->inner_vertices, oct->inner_offsets, oct->position,
                               oct->scale, OCTAGON_INNER_ANGLE,
                               oct->inner_radius);

  return OctagonStatus_Succes;
}
