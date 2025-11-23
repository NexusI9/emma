#include "frame.h"
#include "runtime/manager/allocator.h"
#include "utils/id.h"

FrameStatus frame_create(Frame *node, const FrameDescriptor *desc) {

  frame_set_size(node, desc->size);
  frame_set_position(node, desc->position);
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
