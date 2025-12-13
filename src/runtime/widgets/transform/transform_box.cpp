#include "transform_box.hpp"
#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/unit.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/transform_handle.h"
#include "runtime/widgets/transform/handle.hpp"
#include "runtime/widgets/transform/object_manager.hpp"
#include "runtime/widgets/utils.hpp"
#include "utils/id.h"
#include <climits>
#include <stdint.h>

Widget::Transform::Box::Box(Gui *gui) {

  this->gui = gui;

  for (uint8_t i = 0; i < transform_box_handles_count; i++) {
    TransformHandleDescriptor desc = {
        .position = {0.0f},
        .scale = transform_box_handle_size,
        .color = emma_color(ThemeEmmaColor_Surface_Highest),
        .stroke_width = stroke_width,
        .stroke_color = emma_color(ThemeEmmaColor_Border_Brand_Base),
    };
    transform_handle_create(&handles[i], &desc);
  }

  gui_selection_init(&selection);
}

/**
   Prevents the end from overlapping the start and vice-versa.
   Keeps the bounding box from inverting when dragging handles.
*/
void Widget::Transform::Box::clamp_mouse(const TransformHandleType handle,
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

void Widget::Transform::Box::cache_initial_attributes() {

  // cache initial attributes
  drag_start = vp_im2_scene(ImGui::GetIO().MousePos);
  drag_p0 = p0;
  drag_p1 = p1;

  for (uint16_t obj = 0; obj < object_manager.count(); obj++) {
    ObjectManager::Object *object = object_manager.get_entry(obj);
    object->get_position(object->handle, object->init_position);
    object->get_size(object->handle, object->init_size);
  }
}

void Widget::Transform::Box::transform_core(const TransformHandleType handle) {

  gui_selection_hit(&selection, true);

  ImVec2 mouse = vp_im2_scene(ImGui::GetIO().MousePos);
  clamp_mouse(handle, mouse);

  ImVec2 offset = ImVec2(mouse.x - drag_start.x, mouse.y - drag_start.y);

  offset.x = unit_snap(offset.x);
  offset.y = unit_snap(offset.y);

  for (uint16_t obj = 0; obj < object_manager.count(); obj++) {
    ObjectManager::Object *object = object_manager.get_entry(obj);
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

void Widget::Transform::Box::draw() {

  ImDrawList *draw = ImGui::GetWindowDrawList();

  // Add frame
  draw->AddRect(
      vp_im2(p0), vp_im2(p1),
      im_color((const float *)emma_color(ThemeEmmaColor_Border_Brand_Base)),
      0.0f, 0, stroke_width);

  // Area Behaviour (Translate)
  if ((mode & Mode_Move) && active_handle == -1 &&
      ImGui::IsMouseHoveringRect(vp_im2(padded_area_0),
                                 vp_im2(padded_area_1)) &&
      ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    flag_enable(State_Dragging, &state);
    active_handle = TransformHandleType_MM;
    cache_initial_attributes();
  }

  // Handle Behaviour (Scale)
  if (Mode_Scale & mode)
    for (uint8_t i = 0; i < transform_box_handles_count; i++) {

      if (i == TransformHandleType_MM)
        continue;

      HandleShape handle = HandleShape(&handles[i]);
      handle.draw();

      if (active_handle == -1 &&
          ImGui::IsMouseHoveringRect(handle.get_p0(), handle.get_p1()) &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        flag_enable(State_Dragging, &state);
        active_handle = i;
        cache_initial_attributes();
      }
    }

  if (active_handle >= 0 && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    transform_core((TransformHandleType)active_handle);

  // end session
  if (active_handle >= 0 && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
    active_handle = -1;
    flag_disable(State_Dragging, &state);

    for (size_t i = 0; i < object_manager.count(); i++) {

      ObjectManager::Object *entry = object_manager.get_entry(i);
      if (entry && entry->session_end)
        entry->session_end(entry->handle);
    }
  }
}

void Widget::Transform::Box::handle_transform(const TransformHandleType type,
                                              const ImVec2 init_pos,
                                              const ImVec2 init_size,
                                              const ImVec2 offset,
                                              ImVec2 &new_pos,
                                              ImVec2 &new_size) {

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
Widget::Transform::Box::Status
Widget::Transform::Box::update_bound(ImVec2 start, ImVec2 end) {

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

  return Transform::Box::Status_Success;
}

Widget::Transform::Box::Status
Widget::Transform::Box::update_bound_from_selection() {

  p0 = ImVec2(INFINITY, INFINITY);
  p1 = ImVec2(-INFINITY, -INFINITY);

  for (size_t i = 0; i < object_manager.count(); i++) {
    ImVec2 pos, size;
    ObjectManager::Object *object = object_manager.get_entry(i);
    object->get_position(object->handle, pos);
    object->get_size(object->handle, size);

    p0.x = fminf(p0.x, pos.x);
    p0.y = fminf(p0.y, pos.y);

    p1.x = fmaxf(p1.x, pos.x + size.x);
    p1.y = fmaxf(p1.y, pos.y + size.y);
  }

  update_bound(p0, p1);

  return Transform::Box::Status_Success;
}

Widget::Transform::Box::Status Widget::Transform::Box::begin() {

  gui_selection_begin(&selection,
                      ImGui::IsMouseClicked(ImGuiMouseButton_Right) ||
                          ImGui::IsMouseClicked(ImGuiMouseButton_Left));

  return Transform::Box::Status_Success;
}

Widget::Transform::Box::Status Widget::Transform::Box::end() {

  if (object_manager.count() > 0)
    draw();

  Transform::Box::Status status = Transform::Box::Status_SessionAlreadyStarted;

  if (object_manager.count() &&
      selection_status(&selection) == GuiSelectionStatus_Blank) {
    object_manager.clear();
    status = Transform::Box::Status_ClearSelection;
  }

  selection_end(&selection);

  return status;
}

void Widget::Transform::Box::update_visibility_state() {
  if (objects_count())
    flag_enable(State_Shown, &state);
  else
    flag_disable(State_Shown, &state);
}
