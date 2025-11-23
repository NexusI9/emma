#include "transform_handle.h"

void transform_handle_create(TransformHandle *handle,
                             const TransformHandleDescriptor *desc) {
  glm_vec2_copy((float *)desc->position, handle->position);
  glm_vec4_copy((float *)desc->color, handle->color);
  glm_vec4_copy((float *)desc->stroke_color, handle->stroke_color);

  handle->scale = desc->scale;
  handle->stroke_width = desc->stroke_width;
}
