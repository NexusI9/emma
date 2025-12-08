#include "canvas.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_wgpu.h"
#include "nkengine/include/gui.hpp"
#include "runtime/geometry/boundbox.h"
#include "runtime/geometry/core.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/canvas.h"
#include "runtime/node/connector.h"
#include "runtime/node/connector_handle.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/systems/connect_system.h"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/grid_background.hpp"
#include "runtime/widgets/octagon.hpp"
#include "runtime/widgets/transform_box.hpp"
#include "runtime/widgets/utils.hpp"
#include "utils/id.h"
#include <cstdlib>
#include <imgui/imconfig.h>
#include <imgui/imgui_impl_wgpu.h>

Widget::CanvasShape::CanvasShape(Gui *gui, Canvas *canvas)
    : grid_background("textures/dot-pattern.png", TextureResolution_64),
      canvas_transform(gui, canvas), canvas_interaction(canvas) {

  this->gui = gui;
  this->node = canvas;
}

void Widget::CanvasShape::draw() {

  dl = ImGui::GetWindowDrawList();

  grid_background.draw_texture(gui->pass_encoder);

  // Main Canvas Entities
  canvas_transform.begin();
  {
    draw_pods();
    draw_frames();
    draw_frames_octagon();
    draw_selected_items_connector_handles();
    draw_modules();
    draw_connectors();
  }
  canvas_transform.end();

  // Listeners
  {
    if (canvas_interaction.destroy_listen() == CanvasStatus_Success)
      canvas_transform.transform_box.empty_objects();
  }
}

void Widget::CanvasShape::draw_frame_handle_connectors(Frame *frame,
                                                       const int side) {

  for (uint8_t i = 0; i < FRAME_CONNECTOR_HANDLE_COUNT; i++) {

    // TODO: maybe make a dedicated draw function for side to prevent
    // branching
    if ((side & (1 << i)) == 0)
      continue;

    ConnectorHandle *handle =
        allocator_connector_handle_entry(frame->connector_handle_id[i]);

    ConnectorHandleShape handle_shape =
        ConnectorHandleShape(handle, (ConnectorHandleSide)(1 << i));

    handle_shape.draw();
    canvas_interaction.listen_new_connector_handle(frame, handle,
                                                   &handle_shape);
  }
}

void Widget::CanvasShape::draw_frames_octagon() {

  if ((State_ShowOctagon & state) == 0)
    return;

  for (size_t i = 0; i < node->frames[CanvasFrameState_Octagon].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Octagon].entries[i]);
    OctagonShape(allocator_octagon_entry(frame->octagon_id)).draw();
  }
}

void Widget::CanvasShape::draw_frames() {
  for (size_t i = 0; i < node->frames->length; i++) {
    Frame *frame = allocator_frame_entry(node->frames->entries[i]);
    FrameShape frame_shape = FrameShape(frame);
    frame_shape.draw();
    canvas_transform.listen_frame(&frame_shape,
                                  CanvasTransform::ConfigurationType_Frame);
  }
}
void Widget::CanvasShape::draw_pods() {

  for (size_t i = 0; i < node->pods->length; i++) {
    Frame *pod = allocator_frame_entry(node->pods->entries[i]);
    FrameShape frame_shape = FrameShape(pod);
    frame_shape.draw_pod();
    canvas_interaction.frame_highlight_listen(&frame_shape);
    canvas_transform.listen_frame(&frame_shape,
                                  CanvasTransform::ConfigurationType_Pod);
  }
}
void Widget::CanvasShape::draw_modules() {
  for (size_t i = 0; i < node->modules->length; i++) {
    Frame *module = allocator_frame_entry(node->modules->entries[i]);
    FrameShape frame_shape = FrameShape(module);
    frame_shape.draw_texture();
    canvas_interaction.frame_highlight_listen(&frame_shape);
    canvas_transform.listen_frame(&frame_shape,
                                  CanvasTransform::ConfigurationType_Module);
  }
}

void Widget::CanvasShape::draw_connectors() {

  canvas_interaction.connector_highlight_begin();
  for (size_t i = 0; i < node->connectors->length; i++) {
    Connector *connector =
        allocator_connector_entry(node->connectors->entries[i]);
    ConnectorShape connector_shape = ConnectorShape(gui, connector);

    canvas_interaction.listen_connector_highlight(&connector_shape);
    canvas_interaction.listen_connector_handle_highlight(connector);
    canvas_transform.listen_active_connector_handle(
        canvas_interaction.get_active_connector_handle(), connector);
    canvas_interaction.listen_active_connector_handle_release(connector);

    connector_shape.draw();
  }

  canvas_interaction.connector_highlight_end();
}

void Widget::CanvasShape::draw_selected_items_connector_handles() {

  size_t i;
  for (i = 0; i < node->frames[CanvasFrameState_Selected].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Selected].entries[i]);

    draw_frame_handle_connectors(frame, ConnectorHandleSide_Left |
                                            ConnectorHandleSide_Right);
  }

  for (i = 0; i < node->pods[CanvasPodState_Selected].length; i++) {
    Frame *frame =
        allocator_frame_entry(node->pods[CanvasPodState_Selected].entries[i]);
    draw_frame_handle_connectors(frame, ConnectorHandleSide_Right);
  }

  for (i = 0; i < node->connectors[CanvasConnectorState_Selected].length; i++) {
    Connector *connector = allocator_connector_entry(
        node->connectors[CanvasConnectorState_Selected].entries[i]);

    ConnectorShape(gui, connector).draw_handles();
  }
}
