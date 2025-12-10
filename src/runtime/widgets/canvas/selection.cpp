#include "selection.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/allocator_list.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/canvas.h"
#include "runtime/systems/connect_system.h"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/transform/transform_box.hpp"
#include "runtime/widgets/utils.hpp"

void Widget::Canvas::Selection::frame_selection_listen(
    Frame::Component *frame) {

  if ((state & State_Freeze) == 0 && frame->boundbox_hovered()) {

    ImDrawList *dl = ImGui::GetWindowDrawList();
    ::Frame *frame_node = frame->get_node();
    dl->AddRect(
        ImVec2(vpx(frame_node->world_position[0]),
               vpy(frame_node->world_position[1])),
        ImVec2(vpx(frame_node->end_point[0]), vpy(frame_node->end_point[1])),
        ImColor(im_color(emma_color(ThemeEmmaColor_Border_Brand_Base))), 0, 0,
        Transform::Box::stroke_width);
  }
}

/**
   Detect when we hover one of the connectors touch points/ handles and move it
   with the mouse on click by updating the touch point position coordinate which
   will then affect the draw called upon.
 */
void Widget::Canvas::Selection::listen_connector_handle_selection(
    ::Connector *connector) {

  for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++) {

    ::ConnectorHandle *handle = connector->handles[i];
    ConnectorHandle::Component handle_shape =
        ConnectorHandle::Component(handle, ConnectorHandleSide_None);

    if (handle_shape.hovered()) {

      handle_shape.draw();

      if (gui_selection_hit(&selection_connector,
                            ImGui::IsMouseDown(ImGuiMouseButton_Left))) {

        active_connector_handle = handle;
        flag_enable(State_SelectConnectorHandle, &state);
      }
    }
  }
}

/*
   On Connector handle release we check if the handle is within a frame or pod
   bound and connect it to the closest valid frame/pod handle.
 */
void Widget::Canvas::Selection::listen_active_connector_handle_release(
    ::Connector *connector) {

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
        flag_disable(State_SelectConnectorHandle, &state);
        flag_disable(State_SelectNewConnectorHandle, &state);
        break;
      }
    }
  }
}

void Widget::Canvas::Selection::listen_connector_selection(
    Connector::Component *shape) {

  if (State_Freeze & state)
    return;

  ::Connector *connector = shape->get_node();

  if (gui_selection_hit(&selection_connector,
                        ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
                            shape->clickbox_hovered())) {

    allocator_id_list_empty(
        node->connectors[CanvasConnectorState_Selected].entries,
        &node->connectors[CanvasConnectorState_Selected].length);

    allocator_id_list_push(
        node->connectors[CanvasConnectorState_Selected].entries,
        allocator_connector_capacity(),
        &node->connectors[CanvasConnectorState_Selected].length, connector->id);

    active_connector = connector;
    flag_enable(State_SelectConnector, &state);
    flag_disable(State_Deselect, &state);
  }
}

void Widget::Canvas::Selection::connector_selection_begin() {

  if (gui_selection_begin(&selection_connector,
                          ImGui::IsMouseClicked(ImGuiMouseButton_Left))) {
    flag_disable(State_Deselect, &state);
  }
}

/*
  If didn't catch any connector (active_connector == NULL). Empty the selector
  list.
 */
void Widget::Canvas::Selection::connector_selection_end() {

  if (active_connector &&
      selection_status(&selection_connector) == GuiSelectionStatus_Blank) {

    allocator_id_list_empty(
        node->connectors[CanvasConnectorState_Selected].entries,
        &node->connectors[CanvasConnectorState_Selected].length);

    active_connector = nullptr;
    flag_disable(State_SelectConnector, &state);
    flag_enable(State_Deselect, &state);
  }

  selection_end(&selection_connector);

  if (active_connector_handle &&
      ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
      (State_SelectConnectorHandle & state)) {
    active_connector_handle = nullptr;
    flag_disable(State_SelectConnectorHandle, &state);
  }
}

/**
   Check if we click a frame handle connector to create a new one
 */
void Widget::Canvas::Selection::listen_new_connector_handle(
    ::Frame *frame, ::ConnectorHandle *handle,
    ConnectorHandle::Component *handle_shape) {

  if (gui_selection_hit(&selection_connector,
                        ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                            handle_shape->hovered() && State_None == state)) {

    ConnectorDescriptor connector_desc = {};
    connector_desc.start = handle;
    connector_desc.thickness = CONNECTOR_THICKNESS;
    connector_desc.color = emma_color(ThemeEmmaColor_Border_Subtle_On_Dark);

    ::Connector *new_connector = canvas_create_connector(node, &connector_desc);

    if (new_connector) {
      frame_register_connector(frame, new_connector->id);

      connector_handle_copy(new_connector->handles[0],
                            new_connector->handles[1]);

      active_connector_handle = new_connector->handles[1];

      ImVec2 mouse = vp_im2_scene(ImGui::GetIO().MousePos);
      connector_handle_set_position(active_connector_handle,
                                    (vec2){mouse.x, mouse.y});

      flag_enable(State_SelectNewConnectorHandle, &state);
    }
  }
}
