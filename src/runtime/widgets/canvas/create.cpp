#include "create.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/manager/unit.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/canvas.h"
#include "runtime/node/frame.h"

void Widget::CanvasCreate::listen() {

  if ((state & State_Freeze) == 0) {

    // Clicked based creation
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {

      switch (mode) {

      case Mode_Frame:
        frame_create();
        break;

      case Mode_Pod:
        pod();
        break;

      case Mode_Note:
        note();
        break;

      default:
        break;
      }
    }

    // Drag based creation
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {

      switch (mode) {

      case Mode_Frame:
        frame_resize();
        break;

      case Mode_Module:
        module();
        break;

      case Mode_Shape:
        shape();
        break;

      default:
        break;
      }
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
      frame_release();
    }
  }
}

void Widget::CanvasCreate::get_mouse_position(vec2 dest) {
  glm_vec2(ImGui::GetIO().MousePos, dest);
  vp2_scene(dest, dest);
}

void Widget::CanvasCreate::frame_create() {

  if (new_frame != nullptr)
    return;

  new_frame = canvas_create_frame(node);

  vec2 mouse_pos;
  get_mouse_position(mouse_pos);
  canvas_set_frame_position(node, new_frame, mouse_pos);
}

void Widget::CanvasCreate::frame_resize() {

  if (new_frame == nullptr)
    return;

  vec2 offset, mouse_pos;

  get_mouse_position(mouse_pos);
  glm_vec2_sub(mouse_pos, new_frame->world_position, offset);
  offset[0] = unit_snap(fmaxf(offset[0], FRAME_MIN_SIZE));
  offset[1] = unit_snap(fmaxf(offset[1], FRAME_MIN_SIZE));

  canvas_set_frame_size(node, new_frame, offset);
}

/*
  On release, if the frame size is equal to MIN_SIZE (meaning the cursor
  didn't drag), then we set its size equal to the last created frame in the
  list.
*/
void Widget::CanvasCreate::frame_release() {

  if (!new_frame)
    return;

  if (new_frame->size[0] == FRAME_MIN_SIZE &&
      new_frame->size[1] == FRAME_MIN_SIZE && node->frames->length > 1) {

    Frame *last_frame =
        allocator_frame_entry(node->frames->entries[node->frames->length - 2]);

    canvas_set_frame_size(node, new_frame, last_frame->size);
  }

  new_frame = nullptr;
}

void Widget::CanvasCreate::module() {}

void Widget::CanvasCreate::pod() {

  Frame *pod = canvas_create_pod(node);
  if (pod) {
    vec2 mouse_pos;
    get_mouse_position(mouse_pos);
    // center sprite to cursor
    glm_vec2_sub(mouse_pos, pod_half_size, mouse_pos);
    canvas_set_pod_position(node, pod, mouse_pos);
  }
}

void Widget::CanvasCreate::shape() {}

void Widget::CanvasCreate::note() {}
