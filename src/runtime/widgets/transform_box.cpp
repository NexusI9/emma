#include "transform_box.hpp"
#include "runtime/node/handle.h"
#include "runtime/widgets/handle.hpp"
#include "utils/id.h"
#include <climits>
#include <stdint.h>

Widget::TransformBox::TransformBox(Gui *gui) {

  this->gui = gui;
  for (uint8_t i = 0; i < transform_box_handles_count; i++) {
    HandleDescriptor desc = {
        .position = {0.0f},
        .scale = transform_box_handle_size,
        .color = {1.0f, 1.0f, 1.0f, 1.0f},
    };
    handle_create(&handles[i], &desc);
  }
}

Widget::TransformBoxStatus
Widget::TransformBox::toggle_object(const TransformBoxObjectDescriptor *desc) {

  size_t index;
  if (!find_object(desc->handle, &index))
    add_object(desc);
  else
    remove_object(desc->handle, &index);

  return TransformBoxStatus_Success;
}

Widget::TransformBoxObject *
Widget::TransformBox::find_object(const void *handle, size_t *index) {

  for (size_t i = 0; i < objects.count; i++)
    if (objects.entries[i].handle == handle) {
      if (index)
        *index = i;
      return &objects.entries[i];
    }

  if (index)
    *index = SIZE_MAX;

  return NULL;
}

StaticListStatus
Widget::TransformBox::add_object(const TransformBoxObjectDescriptor *desc) {

  if (objects.count == transform_box_objects_capacity)
    return StaticListStatus_MaxCapacity;

  TransformBoxObject object = {
      .handle = desc->handle,
      .get_position = desc->get_position,
      .set_position = desc->set_position,
      .get_size = desc->get_size,
      .set_size = desc->set_size,
  };

  return stli_insert(objects.entries, transform_box_objects_capacity,
                     &objects.count, sizeof(TransformBoxObject), &object,
                     "Transform Box Target List");
}

StaticListStatus Widget::TransformBox::remove_object(const void *target,
                                                     size_t *index) {

  size_t found_index = index ? (*index) : SIZE_MAX;

  if (index == NULL)
    find_object(target, &found_index);

  if (found_index == SIZE_MAX)
    return StaticListStatus_UnfoundEntry;

  return stli_remove_at_index(objects.entries, &objects.count,
                              sizeof(TransformBoxObject), found_index);
}

StaticListStatus Widget::TransformBox::empty_objects() {

  return stli_empty(objects.entries, &objects.count, sizeof(TransformBoxObject),
                    "Transform Box Target List");
}

/**
   Prevents the end from overlapping the start and vice-versa.
   Keeps the bounding box from inverting when dragging handles.
*/
void Widget::TransformBox::clamp_mouse(const HandleType handle, ImVec2 &dest) {

  static const float margin = 10.0f;

  switch (handle) {

  case HandleType_TL:
    dest.x = fminf(dest.x, p1.x - margin);
    dest.y = fminf(dest.y, p1.y - margin);
    break;

  case HandleType_TM:
    dest.y = fminf(dest.y, p1.y - margin);
    break;

  case HandleType_TR:
    dest.x = fmaxf(dest.x, p0.x + margin);
    dest.y = fminf(dest.y, p1.y - margin);
    break;

  case HandleType_ML:
    dest.x = fminf(dest.x, p1.x - margin);
    break;

  case HandleType_MR:
    dest.x = fmaxf(dest.x, p0.x + margin);
    break;

  case HandleType_BL:
    dest.x = fminf(dest.x, p1.x - margin);
    dest.y = fmaxf(dest.y, p0.y + margin);
    break;

  case HandleType_BM:
    dest.y = fmaxf(dest.y, p0.y + margin);
    break;

  case HandleType_BR:
    dest.x = fmaxf(dest.x, p0.x + margin);
    dest.y = fmaxf(dest.y, p0.y + margin);
    break;

  default:
    break;
  }
}

void Widget::TransformBox::cache_initial_attributes() {

  // cache initial attributes
  drag_start = ImGui::GetIO().MousePos;
  drag_p0 = p0;
  drag_p1 = p1;

  for (uint16_t obj = 0; obj < objects.count; obj++) {
    TransformBoxObject *object = &objects.entries[obj];
    object->get_position(object->handle, object->init_position);
    object->get_size(object->handle, object->init_size);
  }
}

void Widget::TransformBox::transform_core(const HandleType handle) {

  ImVec2 mouse = ImGui::GetIO().MousePos;
  clamp_mouse(handle, mouse);

  ImVec2 offset = ImVec2(mouse.x - drag_start.x, mouse.y - drag_start.y);

  for (uint16_t obj = 0; obj < objects.count; obj++) {
    TransformBoxObject *object = &objects.entries[obj];
    ImVec2 new_pos = object->init_position;
    ImVec2 new_size = object->init_size;
    handle_transform(handle, object->init_position, object->init_size, offset,
                     new_pos, new_size);

    // compensate stretch if use top or right side handles
    switch (handle) {
    case HandleType_TL:
      new_size.x -= offset.x;
      new_size.y -= offset.y;
      break;

    case HandleType_ML:
    case HandleType_BL:
      new_size.x -= offset.x;
      break;

    case HandleType_TM:
    case HandleType_TR:
      new_size.y -= offset.y;
      break;

    case HandleType_MM:
      new_size = object->init_size;
      break;

    default:
      break;
    }

    object->set_position(object->handle, new_pos);
    object->set_size(object->handle, new_size);
  }

  // update frame bound
  ImVec2 new_pos = p0;
  ImVec2 new_size = p1;
  handle_transform(handle, drag_p0, drag_p1, offset, new_pos, new_size);
  update_bound(new_pos, new_size);
}

void Widget::TransformBox::draw() {

  ImDrawList *draw = ImGui::GetWindowDrawList();
  draw->AddRect(p0, p1, IM_COL32(255, 255, 255, 255), 0.0f, 0, 2.0f);

  // Area Behaviour (Translate)
  if (active_handle == -1 &&
      ImGui::IsMouseHoveringRect(padded_area_0, padded_area_1) &&
      ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    active_handle = HandleType_MM;
    cache_initial_attributes();
  }

  // Handle Behaviour (Scale)
  for (uint8_t i = 0; i < transform_box_handles_count; i++) {
    HandleShape handle = HandleShape(&handles[i]);
    handle.draw();

    if (active_handle == -1 &&
        ImGui::IsMouseHoveringRect(handle.get_p0(), handle.get_p1()) &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      active_handle = i;
      cache_initial_attributes();
    }
  }

  if (active_handle >= 0 && ImGui::IsMouseDown(0))
    transform_core((HandleType)active_handle);

  if (active_handle >= 0 && ImGui::IsMouseReleased(0)) {
    active_handle = -1;
  }
}

void Widget::TransformBox::handle_transform(const HandleType type,
                                            const ImVec2 init_pos,
                                            const ImVec2 init_size,
                                            const ImVec2 offset,
                                            ImVec2 &new_pos, ImVec2 &new_size) {

  switch (type) {

  case HandleType_TL: // Top Left
    new_pos = ImVec2(init_pos.x + offset.x, init_pos.y + offset.y);
    new_size = init_size;
    break;

  case HandleType_TM: // Top Middle
    new_pos = ImVec2(init_pos.x, init_pos.y + offset.y);
    new_size = init_size;
    break;

  case HandleType_TR: // Top Right
    new_pos = ImVec2(init_pos.x, init_pos.y + offset.y);
    new_size = ImVec2(init_size.x + offset.x, init_size.y);
    break;

  case HandleType_ML: // Middle Left
    new_pos = ImVec2(init_pos.x + offset.x, init_pos.y);
    new_size = init_size;

    break;

  case HandleType_MM: // Middle Middle
    new_pos = ImVec2(init_pos.x + offset.x, init_pos.y + offset.y);
    new_size = ImVec2(init_size.x + offset.x, init_size.y + offset.y);

    break;

  case HandleType_MR: // Middle Right
    new_pos = init_pos;
    new_size = ImVec2(init_size.x + offset.x, init_size.y);

    break;

  case HandleType_BL: // Bottom Left
    new_pos = ImVec2(init_pos.x + offset.x, init_pos.y);
    new_size = ImVec2(init_size.x, init_size.y + offset.y);
    break;

  case HandleType_BM: // Bottom Middle
    new_pos = init_pos;
    new_size = ImVec2(init_size.x, init_size.y + offset.y);
    break;

  case HandleType_BR: // Bottom Right
    new_pos = init_pos,
    new_size = ImVec2(init_size.x + offset.x, init_size.y + offset.y);
    break;
  }
}

/*
  Position handles according to start and end bound points
 */
Widget::TransformBoxStatus Widget::TransformBox::update_bound(ImVec2 start,
                                                              ImVec2 end) {

  p0 = start;
  p1 = end;

  padded_area_0 = ImVec2(p0.x + area_padding, p0.y + area_padding);
  padded_area_1 = ImVec2(p1.x - area_padding, p1.y - area_padding);

  const float x[] = {
      start.x,
      (start.x + end.x) / 2,
      end.x,
  };

  const float y[] = {
      start.y,
      (start.y + end.y) / 2,
      end.y,
  };

  uint16_t id = 0;
  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      handles[id].position[0] = x[j];
      handles[id].position[1] = y[i];
      id++;
    }
  }

  return TransformBoxStatus_Success;
}

Widget::TransformBoxStatus Widget::TransformBox::update_bound_from_selection() {

  p0 = ImVec2(INFINITY, INFINITY);
  p1 = ImVec2(-INFINITY, -INFINITY);

  for (size_t i = 0; i < objects_count(); i++) {
    ImVec2 pos, size;
    TransformBoxObject *object = &objects.entries[i];
    object->get_position(object->handle, pos);
    object->get_size(object->handle, size);

    p0.x = fminf(p0.x, pos.x);
    p0.y = fminf(p0.y, pos.y);

    p1.x = fmaxf(p1.x, pos.x + size.x);
    p1.y = fmaxf(p1.y, pos.y + size.y);
  }

  update_bound(p0, p1);

  return TransformBoxStatus_Success;
}
