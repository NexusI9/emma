#include "octagon.h"
#include <stdio.h>
#include <string.h>

OctagonStatus octagon_create(Octagon *oct, const OctagonDescriptor *desc) {

  oct->label = desc->label;

  octagon_set_scale(oct, desc->scale);
  octagon_set_outer_color(oct, desc->outer_color);
  octagon_set_inner_color(oct, desc->inner_color);
  octagon_set_inner_radius(oct, desc->inner_radius);
  octagon_set_position(oct, desc->position);

  octagon_update_vertices(oct);

  return OctagonStatus_Success;
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

  return OctagonStatus_Success;
}

OctagonStatus octagon_update_vertices(Octagon *oct) {

  octagon_update_vertices_core(oct->outer_vertices, oct->outer_offsets,
                               oct->position, oct->scale, OCTAGON_OUTER_ANGLE,
                               1.0f);

  octagon_update_vertices_core(oct->inner_vertices, oct->inner_offsets,
                               oct->position, oct->scale, OCTAGON_INNER_ANGLE,
                               oct->inner_radius);

  return OctagonStatus_Success;
}

OctagonStatus octagon_update_labels_coordinates(Octagon *oct) {

  static const float label_margin = 20.0f;

  for (int i = 0; i < OCTAGON_VERTEX_COUNT; i++) {

    float x = oct->outer_vertices[i][0];
    float y = oct->outer_vertices[i][1];

    vec2 sub;
    glm_vec2_sub(oct->outer_vertices[i], oct->position, sub);

    // handle X position adjustments
    if (sub[0] < 0.0f)
      x -= strlen(oct->labels[i]) * 16.0f - label_margin;

    else if (sub[0] == 0.0f)
      x -= strlen(oct->labels[i]) * 5.0f;

    else
      x += label_margin;

    // handle Y position adjustments
    if (sub[1] < 0.0f)
      y -= 60.0f - label_margin;

    else if (sub[1] == 0.0f)
      y -= 10.0f;

    glm_vec2_copy((vec2){x, y}, oct->label_coordinates[i]);
  }

  return OctagonStatus_Success;
}
