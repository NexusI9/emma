#include "transform_box.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/unit.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/transform_handle.h"
#include "runtime/widgets/transform_handle.hpp"
#include "runtime/widgets/utils.hpp"
#include "utils/id.h"
#include <climits>
#include <stdint.h>

Widget::TransformBox::TransformBox(Gui *gui) {

  this->gui = gui;

  for (uint8_t i = 0; i < transform_box_handles_count; i++) {
    TransformHandleDescriptor desc = {
        .position = {0.0f},
        .scale = transform_box_handle_size,
        .color = {1.0f, 1.0f, 1.0f, 1.0f},
        .stroke_width = stroke_width,
        .stroke_color = {primary_color[0], primary_color[1], primary_color[2],
                         primary_color[3]},
    };
    transform_handle_create(&handles[i], &desc);
  }
}

Widget::TransformBoxStatus
Widget::TransformBox::toggle_object(const TransformBoxObjectDescriptor *desc) {

  size_t index;
  if (!find_object(desc->handle, &index)) {
    add_object(desc);
    return TransformBoxStatus_ObjectAdded;
  }

  remove_object(desc->handle, &index);
  return TransformBoxStatus_ObjectRemoved;
}

Widget::TransformBoxObject *
Widget::TransformBox::find_object(const void *handle, size_t *index) {

  for (size_t i = 0; i < objects.count; i++) {

    if (objects.entries[i].handle == handle) {
      if (index)
        *index = i;
      return &objects.entries[i];
    }
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
void Widget::TransformBox::clamp_mouse(const TransformHandleType handle,
                                       ImVec2 &dest) {

  static const float margin = 10.0f;

  switch (handle) {

  case TransformHandleType_TL:
    dest.x = fminf(dest.x, p1.x - margin);
    dest.y = fminf(dest.y, p1.y - margin);
    break;

  case TransformHandleType_TM:
    dest.y = fminf(dest.y, p1.y - margin);
    break;

  case TransformHandleType_TR:
    dest.x = fmaxf(dest.x, p0.x + margin);
    dest.y = fminf(dest.y, p1.y - margin);
    break;

  case TransformHandleType_ML:
    dest.x = fminf(dest.x, p1.x - margin);
    break;

  case TransformHandleType_MR:
    dest.x = fmaxf(dest.x, p0.x + margin);
    break;

  case TransformHandleType_BL:
    dest.x = fminf(dest.x, p1.x - margin);
    dest.y = fmaxf(dest.y, p0.y + margin);
    break;

  case TransformHandleType_BM:
    dest.y = fmaxf(dest.y, p0.y + margin);
    break;

  case TransformHandleType_BR:
    dest.x = fmaxf(dest.x, p0.x + margin);
    dest.y = fmaxf(dest.y, p0.y + margin);
    break;

  default:
    break;
  }
}

/**
   Only update the  session status if the state is OFF, meaning we do change the
   state if this one has be marked as successful (hit).
 */
Widget::TransformBoxStatus Widget::TransformBox::session_set_blank_click() {

  if (session_status == TransformBoxSessionStatus_Off)
    session_status = TransformBoxSessionStatus_BlankClick;

  return TransformBoxStatus_Success;
}

Widget::TransformBoxStatus Widget::TransformBox::session_set_hit() {

  session_status = TransformBoxSessionStatus_Hit;

  return TransformBoxStatus_Success;
}

Widget::TransformBoxStatus Widget::TransformBox::session_end() {

  TransformBoxStatus status = TransformBoxStatus_SessionAlreadyStarted;

  if (session_status == TransformBoxSessionStatus_BlankClick) {
    empty_objects();
    status = TransformBoxStatus_ClearSelection;
  }

  session_status = TransformBoxSessionStatus_Off;

  return status;
}

void Widget::TransformBox::cache_initial_attributes() {

  // cache initial attributes
  drag_start = vp_im2_scene(ImGui::GetIO().MousePos);
  drag_p0 = p0;
  drag_p1 = p1;

  for (uint16_t obj = 0; obj < objects.count; obj++) {
    TransformBoxObject *object = &objects.entries[obj];
    object->get_position(object->handle, object->init_position);
    object->get_size(object->handle, object->init_size);
  }
}

void Widget::TransformBox::transform_core(const TransformHandleType handle) {

  ImVec2 mouse = vp_im2_scene(ImGui::GetIO().MousePos);
  clamp_mouse(handle, mouse);

  ImVec2 offset = ImVec2(mouse.x - drag_start.x, mouse.y - drag_start.y);

  offset.x = unit_snap(offset.x);
  offset.y = unit_snap(offset.y);

  for (uint16_t obj = 0; obj < objects.count; obj++) {
    TransformBoxObject *object = &objects.entries[obj];
    ImVec2 new_pos = object->init_position;
    ImVec2 new_size = object->init_size;

    handle_transform(handle, object->init_position, object->init_size, offset,
                     new_pos, new_size);

    // compensate stretch if use top or right side handles
    switch (handle) {
    case TransformHandleType_TL:
      new_size.x -= offset.x;
      new_size.y -= offset.y;
      break;

    case TransformHandleType_ML:
    case TransformHandleType_BL:
      new_size.x -= offset.x;
      break;

    case TransformHandleType_TM:
    case TransformHandleType_TR:
      new_size.y -= offset.y;
      break;

    case TransformHandleType_MM:
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

  // Add frame
  draw->AddRect(vp_im2(p0), vp_im2(p1), im_color((float *)primary_color), 0.0f,
                0, stroke_width);

  // Area Behaviour (Translate)
  if ((mode & TransformBoxMode_Move) && active_handle == -1 &&
      ImGui::IsMouseHoveringRect(vp_im2(padded_area_0),
                                 vp_im2(padded_area_1)) &&
      ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    active_handle = TransformHandleType_MM;
    cache_initial_attributes();
  }

  // Handle Behaviour (Scale)
  for (uint8_t i = 0; i < transform_box_handles_count; i++) {

    if (TransformBoxMode_All != mode)
      if (((TransformBoxMode_Move & mode) && i != TransformHandleType_MM) ||
          ((TransformBoxMode_Scale & mode) && i == TransformHandleType_MM) ||
          (TransformBoxMode_None & mode))
        continue;

    TransformHandleShape handle = TransformHandleShape(&handles[i]);
    handle.draw();

    if (active_handle == -1 &&
        ImGui::IsMouseHoveringRect(handle.get_p0(), handle.get_p1()) &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      active_handle = i;
      cache_initial_attributes();
    }
  }

  if (active_handle >= 0 && ImGui::IsMouseDown(0))
    transform_core((TransformHandleType)active_handle);

  if (active_handle >= 0 && ImGui::IsMouseReleased(0))
    active_handle = -1;
}

void Widget::TransformBox::handle_transform(const TransformHandleType type,
                                            const ImVec2 init_pos,
                                            const ImVec2 init_size,
                                            const ImVec2 offset,
                                            ImVec2 &new_pos, ImVec2 &new_size) {

  switch (type) {

  case TransformHandleType_TL: // Top Left
    new_pos = ImVec2(init_pos.x + offset.x, init_pos.y + offset.y);
    new_size = init_size;
    break;

  case TransformHandleType_TM: // Top Middle
    new_pos = ImVec2(init_pos.x, init_pos.y + offset.y);
    new_size = init_size;
    break;

  case TransformHandleType_TR: // Top Right
    new_pos = ImVec2(init_pos.x, init_pos.y + offset.y);
    new_size = ImVec2(init_size.x + offset.x, init_size.y);
    break;

  case TransformHandleType_ML: // Middle Left
    new_pos = ImVec2(init_pos.x + offset.x, init_pos.y);
    new_size = init_size;

    break;

  case TransformHandleType_MM: // Middle Middle
    new_pos = ImVec2(init_pos.x + offset.x, init_pos.y + offset.y);
    new_size = ImVec2(init_size.x + offset.x, init_size.y + offset.y);

    break;

  case TransformHandleType_MR: // Middle Right
    new_pos = init_pos;
    new_size = ImVec2(init_size.x + offset.x, init_size.y);

    break;

  case TransformHandleType_BL: // Bottom Left
    new_pos = ImVec2(init_pos.x + offset.x, init_pos.y);
    new_size = ImVec2(init_size.x, init_size.y + offset.y);
    break;

  case TransformHandleType_BM: // Bottom Middle
    new_pos = init_pos;
    new_size = ImVec2(init_size.x, init_size.y + offset.y);
    break;

  case TransformHandleType_BR: // Bottom Right
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
