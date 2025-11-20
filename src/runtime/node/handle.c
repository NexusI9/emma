#include "handle.h"

void handle_create(Handle *handle, const HandleDescriptor *desc) {
  glm_vec2_copy((float *)desc->position, handle->position);
  glm_vec4_copy((float *)desc->color, handle->color);
  handle->scale = desc->scale;
}
