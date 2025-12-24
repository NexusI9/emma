#include "frame.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/node/connector_handle.h"
#include "runtime/node/octagon.h"
#include "utils/id.h"
#include <math.h>
#include <stddef.h>

FrameStatus frame_create(Frame *node, const FrameDescriptor *desc) {

  node->label = desc->label;
  node->clickbox.update_callback = desc->clickbox->update_callback;
  node->clickbox.padding = desc->clickbox->padding;
  node->clickbox.count = desc->clickbox->count;
  node->children.count = 0;

  frame_set_size(node, desc->size);
  frame_set_world_position(node, desc->position);
  frame_set_background(node, desc->background);
  frame_set_uvs(node, desc->uv0, desc->uv1);
  frame_set_parent(node, ID_UNDEFINED);

  node->octagon = ID_UNDEFINED;

  return FrameStatus_Success;
}

StaticListStatus frame_add_child(Frame *parent, const alloc_id id) {

  if (allocator_id_list_find(parent->children.entries, parent->children.count,
                             id) != NULL)
    return StaticListStatus_DuplicateEntry;

  StaticListStatus push =
      allocator_id_list_push(parent->children.entries, FRAME_MAX_CHILDREN,
                             &parent->children.count, id);

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
      allocator_id_list_pop(node->children.entries, &node->children.count, id);

  if (pop == StaticListStatus_Success) {
    Frame *child = allocator_frame_entry(id);
    child->parent = ID_UNDEFINED;
  }

  return pop;
}

StaticListStatus frame_add_solution(Frame *frame, const alloc_id id) {

  return allocator_id_list_push(frame->solutions.entries, FRAME_MAX_SOLUTIONS,
                                &frame->solutions.count, id);
}

StaticListStatus frame_remove_solution(Frame *frame, const alloc_id id) {

  return allocator_id_list_pop(frame->solutions.entries,
                               &frame->solutions.count, id);
}

StaticListStatus frame_add_motivation(Frame *frame, const alloc_id id) {

  return allocator_id_list_push(frame->motivations.entries, FRAME_MAX_SOLUTIONS,
                                &frame->motivations.count, id);
}

StaticListStatus frame_remove_motivation(Frame *frame, const alloc_id id) {

  return allocator_id_list_pop(frame->motivations.entries,
                               &frame->motivations.count, id);
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

  for (size_t i = 0; i < node->children.count; i++) {
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
  node->clickbox.update_callback(node->clickbox.entries,
                                 frame_get_world_position(node),
                                 node->end_point, node->clickbox.padding);
  frame_update_boundbox(node);

  for (size_t i = 0; i < node->children.count; i++) {
    Frame *child = allocator_frame_entry(node->children.entries[i]);
    frame_update_world_position(child);
    child->clickbox.update_callback(child->clickbox.entries,
                                    frame_get_world_position(child),
                                    child->end_point, child->clickbox.padding);
    frame_update_boundbox(child);
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

  for (size_t i = 0; i < node->children.count; i++) {
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

FrameStatus frame_destroy(Frame *frame) {

  if (frame->parent != ID_UNDEFINED) {
    Frame *parent = allocator_frame_entry(frame->parent);
    StaticListStatus pop = allocator_id_list_pop(
        parent->children.entries, &parent->children.count, frame->id);

    frame->parent = ID_UNDEFINED;
  } 

  for (uint8_t i = 0; i < frame->solutions.count; i++) {
    frame_remove_solution(frame, frame->solutions.entries[i]);
    destroy_solution(frame->solutions.entries[i]);
  }

  for (uint8_t i = 0; i < frame->motivations.count; i++) {
    frame_remove_motivation(frame, frame->motivations.entries[i]);
    destroy_motivation(frame->motivations.entries[i]);
  }

  if (frame->octagon != ID_UNDEFINED) {
    destroy_octagon(frame->octagon);
    frame->octagon = ID_UNDEFINED;
  }

  frame->connectors.count = 0;

  // Remove the connectors handle
  for (uint8_t i = 0; i < FRAME_CONNECTOR_HANDLE_COUNT; i++) {
    ConnectorHandle *handle =
        allocator_connector_handle_entry(frame->connector_handle_id[i]);
    if (handle)
      connector_handle_destroy(handle);
  }

  for (size_t i = 0; i < frame->children.count; i++) {
    Frame *child = allocator_frame_entry(frame->children.entries[i]);
    child->parent = ID_UNDEFINED;
  }

  // remove it from the registry/ allocator
  destroy_frame(frame->id);

  return FrameStatus_Success;
}
