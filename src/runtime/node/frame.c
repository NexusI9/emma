#include "frame.h"
#include "runtime/manager/allocator.h"
#include "utils/id.h"
#include <math.h>
#include <stddef.h>

FrameStatus frame_create(Frame *node, const FrameDescriptor *desc) {

  node->label = desc->label;
  node->boundbox.update_callback = desc->boundbox->update_callback;
  node->boundbox.padding = desc->boundbox->padding;
  node->boundbox.count = desc->boundbox->count;
  node->children.length = 0;

  frame_set_size(node, desc->size);
  frame_set_world_position(node, desc->position);
  frame_set_background(node, desc->background);
  frame_set_uvs(node, desc->uv0, desc->uv1);
  frame_set_parent(node, ID_UNDEFINED);

  return FrameStatus_Success;
}

StaticListStatus frame_add_child(Frame *parent, const alloc_id id) {

  StaticListStatus push =
      allocator_id_list_push(parent->children.entries, FRAME_MAX_CHILDREN,
                             &parent->children.length, id);

  if (push != StaticListStatus_Success)
    return push;

  Frame *child = allocator_frame_entry(id);
  child->parent = parent->id;

  glm_vec2_sub(child->world_position, parent->world_position,
               child->local_position);

  return push;
}

StaticListStatus frame_remove_child(Frame *node, const alloc_id id) {

  StaticListStatus pop =
      allocator_id_list_pop(node->children.entries, &node->children.length, id);

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

FrameStatus frame_set_world_position(Frame *node, const vec2 value) {

  if (node->parent != ID_UNDEFINED) {
    Frame *parent = allocator_frame_entry(node->parent);
    glm_vec2_sub((float *)value, parent->world_position, node->local_position);
  } else {
    glm_vec2_copy((float *)value, node->local_position);
  }

  glm_vec2_copy((float *)value, node->world_position);
  glm_vec2_add(node->world_position, node->size, node->end_point);
  node->boundbox.update_callback(node->boundbox.entries,
                                 frame_get_world_position(node),
                                 node->end_point, node->boundbox.padding);

  for (size_t i = 0; i < node->children.length; i++) {
    Frame *child = allocator_frame_entry(node->children.entries[i]);
    frame_update_world_position(child);
    child->boundbox.update_callback(child->boundbox.entries,
                                    frame_get_world_position(child),
                                    child->end_point, child->boundbox.padding);
  }

  return FrameStatus_Success;
}

/**
   Compute children mix and max box size and wrap the frame position and scale
   around it (auto-resize)
 */
FrameStatus frame_wrap(Frame *node) {

  vec2 children_start = {INFINITY, INFINITY};
  vec2 children_end = {-INFINITY, -INFINITY};

  for (size_t i = 0; i < node->children.length; i++) {
    Frame *child = allocator_frame_entry(node->children.entries[i]);

    children_start[0] = fminf(children_start[0], child->world_position[0]);
    children_start[1] = fminf(children_start[1], child->world_position[1]);

    children_end[0] = fmaxf(children_end[0], child->end_point[0]);
    children_end[1] = fmaxf(children_end[1], child->end_point[1]);
  }

  frame_set_local_position(node, children_start);
  glm_vec2_sub(children_end, node->world_position, children_end);
  frame_set_size(node, children_end);

  return FrameStatus_Success;
}
