#include "selection.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/viewport.h"
#include "runtime/node/canvas.h"
#include "runtime/systems/connect_system.h"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/transform_box.hpp"
#include "runtime/widgets/utils.hpp"

void Widget::CanvasSelection::frame_selection_listen(FrameShape *frame) {

  if ((selection_state & SelectionState_Freeze) == 0 &&
      frame->boundbox_hovered()) {

    ImDrawList *dl = ImGui::GetWindowDrawList();
    Frame *frame_node = frame->get_node();
    dl->AddRect(
        ImVec2(vpx(frame_node->world_position[0]),
               vpy(frame_node->world_position[1])),
        ImVec2(vpx(frame_node->end_point[0]), vpy(frame_node->end_point[1])),
        ImColor(im_color(TransformBox::primary_color)), 0, 0,
        TransformBox::stroke_width);
  }
}

/**
   Detect when we hover one of the connectors touch points/ handles and move it
   with the mouse on click by updating the touch point position coordinate which
   will then affect the draw called upon.
 */
void Widget::CanvasSelection::listen_connector_handle_selection(
    Connector *connector) {

  for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++) {

    ConnectorHandle *handle = connector->handles[i];
    ConnectorHandleShape handle_shape =
        ConnectorHandleShape(handle, ConnectorHandleSide_None);

    if (handle_shape.hovered()) {

      handle_shape.draw();

      if (gui_selection_hit(&selection_connector,
                            ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                                SelectionState_None == selection_state)) {
        active_connector_handle = handle;
        flag_enable(SelectionState_ConnectorHandle, &selection_state);
      }
    }
  }
}

/*
   On Connector handle release we check if the handle is within a frame or pod
   bound and connect it to the closest valid frame/pod handle.
 */
void Widget::CanvasSelection::listen_active_connector_handle_release(
    Connector *connector) {

  if (active_connector_handle &&
      ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {

    struct {
      FrameAllocList *list;
      const unsigned int sides;
    } frames[] = {
        {node->frames, ConnectorHandleSide_Left | ConnectorHandleSide_Right},
        {node->pods, ConnectorHandleSide_Right},
    };

    static const uint8_t release_frames_count =
        sizeof(frames) / sizeof(frames[0]);

    for (uint8_t i = 0; i < release_frames_count; i++) {
      if (connect_system_connect_handle_to_frame(
              active_connector_handle, connector, frames[i].list->entries,
              frames[i].list->length,
              frames[i].sides) == ConnectSystemStatus_Success) {
        active_connector_handle = nullptr;
        flag_disable(SelectionState_ConnectorHandle, &selection_state);
        flag_disable(SelectionState_NewConnectorHandle, &selection_state);
        break;
      }
    }
  }
}

void Widget::CanvasSelection::listen_connector_selection(
    ConnectorShape *shape) {

  if (SelectionState_Freeze & selection_state)
    return;

  Connector *connector = shape->get_node();

  if (gui_selection_hit(&selection_connector,
                        ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
                            shape->clickbox_hovered())) {

    allocator_id_list_push(
        node->connectors[CanvasConnectorState_Selected].entries,
        allocator_connector_capacity(),
        &node->connectors[CanvasConnectorState_Selected].length, connector->id);
    active_connector = connector;
  }
}

void Widget::CanvasSelection::connector_selection_begin() {

  gui_selection_begin(&selection_connector,
                      ImGui::IsMouseClicked(ImGuiMouseButton_Left));
}

/*
  If didn't catch any connector (active_connector == NULL). Empty the selector
  list.
 */
void Widget::CanvasSelection::connector_selection_end() {

  if (selection_status(&selection_connector) == GuiSelectionStatus_Blank) {
    stli_empty(node->connectors[CanvasConnectorState_Selected].entries,
               &node->connectors[CanvasConnectorState_Selected].length,
               sizeof(alloc_id), "Canvas Selected Connectors List");
    active_connector = nullptr;
  }

  selection_end(&selection_connector);

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
      (SelectionState_ConnectorHandle & selection_state)) {
    active_connector_handle = nullptr;
    flag_disable(SelectionState_ConnectorHandle, &selection_state);
  }
}

/**
   Check if we click a frame handle connector to create a new one
 */
void Widget::CanvasSelection::listen_new_connector_handle(
    Frame *frame, ConnectorHandle *handle, ConnectorHandleShape *handle_shape) {

  if (gui_selection_hit(&selection_connector,
                        ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                            handle_shape->hovered() &&
                            SelectionState_None == selection_state)) {

    ConnectorDescriptor connector_desc = {};
    connector_desc.start = handle;
    connector_desc.thickness = CONNECTOR_THICKNESS;
    connector_desc.color = CONNECTOR_COLOR;

    Connector *new_connector = canvas_create_connector(node, &connector_desc);

    if (new_connector) {
      frame_register_connector(frame, new_connector->id);

      connector_handle_copy(new_connector->handles[0],
                            new_connector->handles[1]);

      active_connector_handle = new_connector->handles[1];

      ImVec2 mouse = vp_im2_scene(ImGui::GetIO().MousePos);
      connector_handle_set_position(active_connector_handle,
                                    (vec2){mouse.x, mouse.y});

      flag_enable(SelectionState_NewConnectorHandle, &selection_state);
    }
  }
}
