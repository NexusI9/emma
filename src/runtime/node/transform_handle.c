#include "transform_handle.h"

void transform_handle_create(TransformHandle *handle,
                             const TransformHandleDescriptor *desc) {
  glm_vec2_copy((float *)desc->position, handle->position);
  handle->color = desc->color;
  handle->stroke_color = desc->stroke_color;
  handle->scale = desc->scale;
  handle->stroke_width = desc->stroke_width;
}
