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
#include "runtime/widgets/canvas.hpp"
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
      transform_box(gui) {

  this->gui = gui;
  this->node = canvas;

  gui_selection_init(&selection_connector);
  gui_highlight_init(&highlight);

  transform_box.update_bound(ImVec2(20, 20), ImVec2(900, 300));

  {
    // Frame transform config
    CanvasTransformConfiguration *fm_conf =
        &transform_configuration[CanvasTransformConfigurationType_Frame];

    fm_conf->transform_mode = TransformBoxMode_All;
    fm_conf->get_position = canvas_shape_get_frame_position;
    fm_conf->set_position = canvas_shape_set_frame_position;
    fm_conf->get_size = canvas_shape_get_frame_size;
    fm_conf->set_size = canvas_shape_set_frame_size;
    fm_conf->selection_list = &node->frames[CanvasFrameState_Selected];
    fm_conf->button = ImGuiMouseButton_Right;
    // fm_conf->parent_list = (group list)
  }

  {
    // Module transform config
    CanvasTransformConfiguration *md_conf =
        &transform_configuration[CanvasTransformConfigurationType_Module];

    md_conf->transform_mode = TransformBoxMode_Move;
    md_conf->get_position = canvas_shape_get_frame_position;
    md_conf->get_size = canvas_shape_get_frame_size;
    md_conf->set_position = canvas_shape_set_module_position;
    md_conf->set_size = canvas_shape_set_module_size;
    md_conf->session_end = canvas_shape_on_module_session_end;
    md_conf->selection_list = &node->modules[CanvasModuleState_Selected];
    md_conf->parent_list = &node->frames[CanvasModuleState_Default];
    md_conf->button = ImGuiMouseButton_Left;
  }

  {
    // Pod transform config
    CanvasTransformConfiguration *pod_conf =
        &transform_configuration[CanvasTransformConfigurationType_Pod];

    pod_conf->transform_mode = TransformBoxMode_Move;
    pod_conf->get_position = canvas_shape_get_frame_position;
    pod_conf->set_position = canvas_shape_set_pod_position;
    pod_conf->get_size = canvas_shape_get_frame_size;
    pod_conf->set_size = canvas_shape_set_pod_size;
    pod_conf->selection_list = &node->pods[CanvasPodState_Selected];
    pod_conf->button = ImGuiMouseButton_Left;
  }
}

/**
   Sync the nodes from the canvas
 */
void sync_nodes() {}

/**
    ▗▄▄▄ ▗▄▄▖  ▗▄▖ ▗▖ ▗▖
    ▐▌  █▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌
    ▐▌  █▐▛▀▚▖▐▛▀▜▌▐▌ ▐▌
    ▐▙▄▄▀▐▌ ▐▌▐▌ ▐▌▐▙█▟▌

 */

void Widget::CanvasShape::draw(const unsigned int flags) {

  dl = ImGui::GetWindowDrawList();

  grid_background.draw_texture(gui->pass_encoder);

  // Main Canvas Entities
  transform_box.begin();
  {
    draw_pods(flags);
    draw_frames(flags);
    draw_frames_octagons(flags);
    draw_selected_items_connector_handles(flags);
    draw_modules(flags);
    draw_connectors(flags);
  }
  if (transform_box.end() == TransformBoxStatus_ClearSelection) {
    canvas_empty_frame_state(node, CanvasFrameState_Selected);
    canvas_empty_pod_state(node, CanvasPodState_Selected);
  }

  // Listeners
  { destroy_listen(); }
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

    if (gui_selection_hit(&selection_connector,
                          ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                              handle_shape.hovered() &&
                              CanvasSelectionFlag_None == selection_state)) {

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

        flag_enable(CanvasSelectionFlag_NewConnectorHandle, &selection_state);
      }
    }
  }
}

void Widget::CanvasShape::draw_frames(const unsigned int flags) {

  for (size_t i = 0; i < node->frames[CanvasFrameState_Default].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Default].entries[i]);

    FrameShape frame_shape = FrameShape(frame);
    frame_shape.draw();

    if ((CanvasDrawFlag_FreezeSelection & flags) == 0)
      frame_transform_listen(
          &frame_shape,
          &transform_configuration[CanvasTransformConfigurationType_Frame]);
  }
}

void Widget::CanvasShape::draw_pods(const unsigned int flags) {

  for (size_t i = 0; i < node->pods[CanvasPodState_Default].length; i++) {

    Frame *frame =
        allocator_frame_entry(node->pods[CanvasPodState_Default].entries[i]);

    FrameShape frame_shape = FrameShape(frame);
    frame_shape.draw_pod();

    if ((CanvasDrawFlag_FreezeSelection & flags) == 0) {
      frame_highlight_listen(&frame_shape);
      frame_transform_listen(
          &frame_shape,
          &transform_configuration[CanvasTransformConfigurationType_Pod]);
    }
  }
}

void Widget::CanvasShape::draw_frames_octagons(const unsigned int flags) {

  if ((CanvasDrawFlag_ShowOctagon & flags) == 0)
    return;

  for (size_t i = 0; i < node->frames[CanvasFrameState_Octagon].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Octagon].entries[i]);

    OctagonShape(allocator_octagon_entry(frame->octagon_id)).draw();
  }
}

void Widget::CanvasShape::draw_selected_items_connector_handles(
    const unsigned int flags) {

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
void Widget::CanvasShape::draw_modules(const unsigned int flags) {

  for (size_t i = 0; i < node->modules[CanvasModuleState_Default].length; i++) {
    Frame *module = allocator_frame_entry(
        node->modules[CanvasModuleState_Default].entries[i]);

    FrameShape frame_shape = FrameShape(module);
    frame_shape.draw_texture();

    if ((CanvasDrawFlag_FreezeSelection & flags) == 0) {
      frame_highlight_listen(&frame_shape);
      frame_transform_listen(
          &frame_shape,
          &transform_configuration[CanvasTransformConfigurationType_Module]);
    }
  }
}
void Widget::CanvasShape::draw_connectors(const unsigned int flags) {

  connector_highlight_begin();

  // Default State
  for (size_t i = 0; i < node->connectors->length; i++) {
    Connector *connector =
        allocator_connector_entry(node->connectors->entries[i]);

    ConnectorShape connector_shape = ConnectorShape(gui, connector);

    if ((CanvasDrawFlag_FreezeSelection & flags) == 0) {
      connector_highlight_listen(&connector_shape);
      connector_handle_transform_listen(connector);
      connector_handle_transform_end(connector);
    }

    connector_shape.draw();
  }

  connector_highlight_end();

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
      (CanvasSelectionFlag_ConnectorHandle & selection_state)) {
    active_connector_handle = nullptr;
    flag_disable(CanvasSelectionFlag_ConnectorHandle, &selection_state);
  }
}

/*


    ▗▖   ▗▄▄▄▖ ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖  ▗▄▄▖
    ▐▌     █  ▐▌     █  ▐▌   ▐▛▚▖▐▌▐▌   ▐▌ ▐▌▐▌
    ▐▌     █   ▝▀▚▖  █  ▐▛▀▀▘▐▌ ▝▜▌▐▛▀▀▘▐▛▀▚▖ ▝▀▚▖
    ▐▙▄▄▖▗▄█▄▖▗▄▄▞▘  █  ▐▙▄▄▖▐▌  ▐▌▐▙▄▄▖▐▌ ▐▌▗▄▄▞▘



 */

/**
   Handle the boundbox interaction along with the transformation for frames
   and modules.
 */
void Widget::CanvasShape::frame_transform_listen(
    FrameShape *frame, const CanvasTransformConfiguration *conf) {

  Frame *frame_node = frame->get_node();

  // no matter the button, if a click happened and hit a frame *area*, we update
  // the transform session status to "Has Hit"
  gui_selection_hit(&transform_box.selection,
                    (ImGui::IsMouseClicked(ImGuiMouseButton_Right) ||
                     ImGui::IsMouseClicked(ImGuiMouseButton_Left)) &&
                        frame->boundbox_hovered());

  // but we only add to selection if the button actually matches the
  // configuration button and hit the
  if (ImGui::IsMouseClicked(conf->button) && frame->boundbox_hovered()) {

    CanvasTransformFrameData data;
    data.frame = frame_node;
    data.canvas = node;
    data.parent_list = conf->parent_list;

    // register the frame for transform callbacks
    if (stli_insert(transform_frame_data.entries, ALLOCATOR_MAX_FRAMES,
                    &transform_frame_data.count,
                    sizeof(CanvasTransformFrameData), &data,
                    "Canvas Transform Frame Data") != StaticListStatus_Success)
      return;

    TransformBoxObjectDescriptor object = {
        .handle = &transform_frame_data.entries[transform_frame_data.count - 1],
        .get_position = conf->get_position,
        .set_position = conf->set_position,
        .get_size = conf->get_size,
        .set_size = conf->set_size,
        .session_end = conf->session_end,
    };

    TransformBoxObject *found_obj =
        transform_box.find_object(object.handle, NULL);

    // remove object
    if (found_obj) {

      // mark as unselected
      allocator_id_list_pop(conf->selection_list->entries,
                            &conf->selection_list->length, frame_node->id);

      transform_box.remove_object(found_obj->handle, NULL);

      // remove it from the cached callback data
      for (size_t i = 0; i < transform_frame_data.count; i++)
        if (transform_frame_data.entries[i].frame == frame_node)
          stli_remove_at_index(transform_frame_data.entries,
                               &transform_frame_data.count,
                               sizeof(CanvasTransformFrameData), i, NULL);

      // add object
    } else {

      // prevent selecting the inner content of the frame when the frame is
      // already selected, in the case  we click inside (on the content) to
      // move the frame (and not selecting the content).
      if (transform_box.objects_count() &&
          transform_box.button != conf->button) {
        return;
      }

      transform_box.mode = conf->transform_mode;
      transform_box.button = conf->button;

      // if not CAP input or if the configuration's Transform Mode is
      // empty and different from the current one, we empty the selection.
      if (input_key(INPUT_KEY_SHIFT) == false) {
        transform_box.empty_objects();
        canvas_empty_frame_state(node, CanvasFrameState_Selected);
      }

      // mark as selected
      allocator_id_list_push(conf->selection_list->entries,
                             ALLOCATOR_MAX_FRAMES,
                             &conf->selection_list->length, frame_node->id);

      transform_box.add_object(&object);
    }

    transform_box.update_bound_from_selection();
  }
}

void Widget::CanvasShape::frame_highlight_listen(FrameShape *frame) {

  if (frame->boundbox_hovered()) {

    Frame *frame_node = frame->get_node();
    dl->AddRect(
        ImVec2(vpx(frame_node->world_position[0]),
               vpy(frame_node->world_position[1])),
        ImVec2(vpx(frame_node->end_point[0]), vpy(frame_node->end_point[1])),
        ImColor(im_color(transform_box.primary_color)), 0, 0,
        transform_box.stroke_width);
  }
}

/**
   Detect when we hover one of the connectors touch points/ handles and move it
   with the mouse on click by updating the touch point position coordinate which
   will then affect the draw called upon.
 */
void Widget::CanvasShape::connector_handle_transform_listen(
    Connector *connector) {

  for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++) {

    ConnectorHandle *handle = connector->handles[i];
    ConnectorHandleShape handle_shape =
        ConnectorHandleShape(handle, ConnectorHandleSide_None);

    if (handle_shape.hovered()) {

      handle_shape.draw();

      if (gui_selection_hit(&selection_connector,
                            ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                                CanvasSelectionFlag_None == selection_state)) {
        active_connector_handle = handle;
        flag_enable(CanvasSelectionFlag_ConnectorHandle, &selection_state);
      }
    }
  }
}

/*
   In this function we check if a handle either from an existing connector or a
   newly created one is active and transform it according to the mouse position.

   Note that we need to use 2 distinct flags for 'existing' and 'newly created'
   connectors handle. Respectively: active_connector_handle and
   active_new_connector_handle.

   The reason to this is that when we create a new connector from a frame
   handle, the end handle overlap with the start handle since the mouse hovered
   the start to trigger the new connector.

   However when we check if the mouse is over an existing connector handle, it
   checks them in ored from 0 -> 1 (start END end). And since the mouse is
   hovering the start AND end the loop order makes that it triggers the start
   handle first instead of the end handle.

   On Connector handle release we check if the handle is within a frame or pod
   bound and connect it to the closest valid frame/pod handle.
 */
void Widget::CanvasShape::connector_handle_transform_end(Connector *connector) {

  if (active_connector_handle) {

    ImVec2 mouse = vp_im2_scene(ImGui::GetIO().MousePos);
    connector_handle_set_position(active_connector_handle,
                                  (vec2){mouse.x, mouse.y});
    connector_update_corners(connector);

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {

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
          flag_disable(CanvasSelectionFlag_ConnectorHandle, &selection_state);
          flag_disable(CanvasSelectionFlag_NewConnectorHandle,
                       &selection_state);
          break;
        }
      }
    }
  }
}

void Widget::CanvasShape::connector_highlight_listen(
    ConnectorShape *connector) {

  if (gui_selection_hit(&selection_connector,
                        ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
                            connector->clickbox_hovered())) {
    active_connector = connector->get_node();

    allocator_id_list_push(
        node->connectors[CanvasConnectorState_Selected].entries,
        allocator_connector_capacity(),
        &node->connectors[CanvasConnectorState_Selected].length,
        active_connector->id);
  }
}

void Widget::CanvasShape::connector_highlight_begin() {

  gui_selection_begin(&selection_connector,
                      ImGui::IsMouseClicked(ImGuiMouseButton_Left));
}

/*
  If didn't catch any connector (active_connector == NULL). Empty the selector
  list.
 */
void Widget::CanvasShape::connector_highlight_end() {

  if (selection_status(&selection_connector) == GuiSelectionStatus_Blank)
    stli_empty(node->connectors[CanvasConnectorState_Selected].entries,
               &node->connectors[CanvasConnectorState_Selected].length,
               sizeof(alloc_id), "Canvas Selected Connectors List");

  selection_end(&selection_connector);
}

void Widget::CanvasShape::destroy_listen() {

  if (input_key(INPUT_KEY_BACKSPACE)) {

    if (active_connector) {
      canvas_destroy_connector(node, active_connector);
      active_connector = nullptr;
    }

    if (canvas_destroy_all_selected_frames(node) == CanvasStatus_Success)
      transform_box.empty_objects();
  }
}

/*

    ▗▄▄▄▖▗▄▄▖  ▗▄▖ ▗▖  ▗▖ ▗▄▄▖▗▄▄▄▖ ▗▄▖ ▗▄▄▖ ▗▖  ▗▖
      █  ▐▌ ▐▌▐▌ ▐▌▐▛▚▖▐▌▐▌   ▐▌   ▐▌ ▐▌▐▌ ▐▌▐▛▚▞▜▌
      █  ▐▛▀▚▖▐▛▀▜▌▐▌ ▝▜▌ ▝▀▚▖▐▛▀▀▘▐▌ ▐▌▐▛▀▚▖▐▌  ▐▌
      █  ▐▌ ▐▌▐▌ ▐▌▐▌  ▐▌▗▄▄▞▘▐▌   ▝▚▄▞▘▐▌ ▐▌▐▌  ▐▌

       ▗▄▄▖ ▗▄▖ ▗▖   ▗▖   ▗▄▄▖  ▗▄▖  ▗▄▄▖▗▖ ▗▖ ▗▄▄▖
      ▐▌   ▐▌ ▐▌▐▌   ▐▌   ▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌▗▞▘▐▌
      ▐▌   ▐▛▀▜▌▐▌   ▐▌   ▐▛▀▚▖▐▛▀▜▌▐▌   ▐▛▚▖  ▝▀▚▖
      ▝▚▄▄▖▐▌ ▐▌▐▙▄▄▖▐▙▄▄▖▐▙▄▞▘▐▌ ▐▌▝▚▄▄▖▐▌ ▐▌▗▄▄▞▘


 */

void Widget::canvas_shape_get_frame_position(void *data, ImVec2 &value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  const float *world_pos = frame_get_world_position(frame);
  value = ImVec2(world_pos[0], world_pos[1]);
}

void Widget::canvas_shape_get_frame_size(void *data, ImVec2 &value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  value = im_vec2(frame->size);
}

void Widget::canvas_shape_set_frame_position(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;
  canvas_set_frame_position(frame_data->canvas, frame,
                            (vec2){value.x, value.y});

  canvas_update_frame_connectors(frame_data->canvas, frame_data->frame);
}

void Widget::canvas_shape_set_frame_size(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_frame_size(frame_data->canvas, frame, (vec2){value.x, value.y});
}

void Widget::canvas_shape_set_module_size(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_module_size(frame_data->canvas, frame, (vec2){value.x, value.y});
}

void Widget::canvas_shape_set_module_position(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_module_world_position(frame_data->canvas, frame,
                                   (vec2){value.x, value.y});

  /*
    Since we crop the frames children, we need to check if the module is
   still within the parent area when moving cause if we move the module out
   of the parent, it will still disapear being cropped out by the parent
   frame.

    As a result to make the user understand that the frame is being
   'unlinked' from parent, we need to remove the parent from the child so it
   is not cropped anymore. However note that we do NOT relink back the child
   here cause on transform session end, we already traverse all the frames
   to check if the module is within one of them.

   Also it would by costly to check on every frame here if the child is within
   any frame node; that's why we only handle the unlink phase here as it is fast
   to target the parent (node->parent) and contribute greatly to the user
   experience.
   */
  if (frame_data->frame->parent != ID_UNDEFINED) {
    Frame *parent = allocator_frame_entry(frame->parent);
    if (!frame_collide(parent, frame))
      frame_remove_child(parent, frame->id);
  }
}

void Widget::canvas_shape_set_pod_size(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_pod_size(frame_data->canvas, frame, (vec2){value.x, value.y});
}

void Widget::canvas_shape_set_pod_position(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_pod_position(frame_data->canvas, frame, (vec2){value.x, value.y});
  canvas_update_frame_connectors(frame_data->canvas, frame_data->frame);
}

/**
   On Mouse release we check if some frames that have parent moved out of the
   parent bound to break the relationship. And and vice versa, i.e. if a child
   is included in a parent bound then add it as child.
 */
void Widget::canvas_shape_on_module_session_end(void *data) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = allocator_frame_entry(frame_data->frame->id);

  // check if still within parent bound
  if (frame_data->frame->parent != ID_UNDEFINED) {

    Frame *parent = allocator_frame_entry(frame->parent);

    if (!frame_collide(parent, frame))
      frame_remove_child(parent, frame->id);
  }

  // check if the frame is within a parent frame scope
  if (frame_data->parent_list)
    for (size_t i = 0; i < frame_data->parent_list->length; i++) {
      Frame *parent =
          allocator_frame_entry(frame_data->parent_list->entries[i]);
      if (frame_collide(parent, frame)) {
        frame_add_child(parent, frame->id);
      }
    }
}
