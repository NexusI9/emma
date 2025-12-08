#include "create.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/node/canvas.h"

void Widget::CanvasCreate::listen() {

  if ((state & State_Freeze) == 0 &&
      ImGui::IsMouseDown(ImGuiMouseButton_Left) && !active) {

    active = true;
    switch (mode) {

    case Mode_Frame:
      frame();
      break;

    case Mode_Module:
      module();
      break;

    case Mode_Pod:
      pod();
      break;

    case Mode_Shape:
      shape();
      break;

    case Mode_Note:
      note();
      break;

    default:
      break;
    }
  }

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && active) {
    active = false;
  }
}

void Widget::CanvasCreate::frame() {}

void Widget::CanvasCreate::module() {}

void Widget::CanvasCreate::pod() {

  Frame *pod = canvas_create_pod(node);
  if (pod) {
    vec2 mouse_pos;
    glm_vec2(ImGui::GetIO().MousePos, mouse_pos);
    canvas_set_pod_position(node, pod, mouse_pos);
  }
}

void Widget::CanvasCreate::shape() {}

void Widget::CanvasCreate::note() {}
