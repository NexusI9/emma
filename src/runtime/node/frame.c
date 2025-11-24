#include "frame.h"
#include "runtime/manager/allocator.h"
#include "utils/id.h"

FrameStatus frame_create(Frame *node, const FrameDescriptor *desc) {

  frame_set_size(node, desc->size);
  frame_set_local_position(node, desc->position);
  frame_set_background(node, desc->background);
  frame_set_uvs(node, desc->uv0, desc->uv1);
  frame_set_parent(node, ID_UNDEFINED);

  return FrameStatus_Success;
}

StaticListStatus frame_add_child(Frame *node, const alloc_id id) {

  StaticListStatus push = allocator_id_list_push(
      node->children.entries, FRAME_MAX_CHILDREN, &node->children.length, id);

  if (push == StaticListStatus_Success) {
    Frame *child = allocator_frame_entry(id);
    child->parent = node->id;
  }

  return push;
}

StaticListStatus frame_remove_child(Frame *node, const alloc_id id) {

  StaticListStatus pop = allocator_id_list_pop(node->children.entries,
                                               &node->connectors_id.length, id);

  if (pop == StaticListStatus_Success) {
    Frame *child = allocator_frame_entry(id);
    child->parent = ID_UNDEFINED;
  }

  return pop;
}

FrameStatus frame_update_world_position(Frame *node) {

  if (node->parent != ID_UNDEFINED) {
    Frame *parent = allocator_frame_entry(node->parent);
    glm_vec2_add(parent->world_position, node->local_position,
                 node->world_position);
  } else {
    glm_vec2_copy(node->local_position, node->world_position);
  }

  glm_vec2_add(node->world_position, node->size, node->end_point);

  for (size_t i = 0; i < node->children.length; i++) {
    Frame *child = allocator_frame_entry(node->children.entries[i]);
    frame_update_world_position(child);
  }

  return FrameStatus_Success;
}
