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
}

/**
   Handle the boundbox interaction along with the transformation for frames
   and modules.
 */
void Widget::CanvasShape::draw_frame_transform_trigger(
    FrameShape *frame, const CanvasTransformConfiguration *conf) {

  Frame *frame_node = frame->get_node();

  // no matter the button, if a click happened and hit a frame *area*, we update
  // the transform session status to "Has Hit"
  if ((ImGui::IsMouseClicked(ImGuiMouseButton_Right) ||
       ImGui::IsMouseClicked(ImGuiMouseButton_Left)) &&
      frame->area_hovered())
    transform_box.session_set_hit();

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
                               sizeof(CanvasTransformFrameData), i);

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
      if (input_key(INPUT_KEY_CAP) == false) {
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

void Widget::CanvasShape::draw_frame_highlight_trigger(FrameShape *frame) {

  if (frame->area_hovered()) {

    Frame *frame_node = frame->get_node();
    dl->AddRect(
        ImVec2(vpx(frame_node->world_position[0]),
               vpy(frame_node->world_position[1])),
        ImVec2(vpx(frame_node->end_point[0]), vpy(frame_node->end_point[1])),
        ImColor(im_color(transform_box.primary_color)), 0, 0,
        transform_box.stroke_width);
  }
}

void Widget::CanvasShape::draw_frame_handle_connectors(Frame *frame,
                                                       const int side) {

  for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++) {

    // TODO: maybe make a dedicated draw function for side to prevent
    // branching
    if ((__builtin_ctz(side) & i) == 0)
      continue;

    ConnectorHandle *handle =
        allocator_connector_handle_entry(frame->connector_handle_id[i]);
    ConnectorHandleShape(handle, (ConnectorHandleSide)(1 << i)).draw();
  }
}

void Widget::CanvasShape::draw(const unsigned int flags) {

  dl = ImGui::GetWindowDrawList();

  grid_background.draw_texture(gui->pass_encoder);

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) ||
      ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    transform_box.session_set_blank_click();

  {
    draw_frames(flags);
    draw_frames_octagons(flags);
    draw_frames_connector_handles(flags);
    draw_modules(flags);
    draw_connectors(flags);
  }

  if (transform_box.session_end() == TransformBoxStatus_ClearSelection)
    canvas_empty_frame_state(node, CanvasFrameState_Selected);

  if (transform_box.objects_count() > 0)
    transform_box.draw();
}

void Widget::CanvasShape::draw_frames(const unsigned int flags) {

  for (size_t i = 0; i < node->frames[CanvasFrameState_Default].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Default].entries[i]);

    FrameShape frame_shape = FrameShape(frame);
    frame_shape.draw();

    if ((CanvasDrawFlag_FreezeSelection & flags) == 0)
      draw_frame_transform_trigger(
          &frame_shape,
          &transform_configuration[CanvasTransformConfigurationType_Frame]);
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

void Widget::CanvasShape::draw_frames_connector_handles(
    const unsigned int flags) {

  for (size_t i = 0; i < node->frames[CanvasFrameState_Selected].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Selected].entries[i]);
    draw_frame_handle_connectors(frame, ConnectorHandleSide_Left |
                                            ConnectorHandleSide_Right);
  }
}
void Widget::CanvasShape::draw_modules(const unsigned int flags) {

  for (size_t i = 0; i < node->modules[CanvasModuleState_Default].length; i++) {
    Frame *module = allocator_frame_entry(
        node->modules[CanvasModuleState_Default].entries[i]);

    FrameShape frame_shape = FrameShape(module);
    frame_shape.draw_texture();

    if ((CanvasDrawFlag_FreezeSelection & flags) == 0) {
      draw_frame_highlight_trigger(&frame_shape);
      draw_frame_transform_trigger(
          &frame_shape,
          &transform_configuration[CanvasTransformConfigurationType_Module]);
    }
  }
}
void Widget::CanvasShape::draw_connectors(const unsigned int flags) {

  // Default State
  for (size_t i = 0; i < node->connectors.length; i++) {
    Connector *connector =
        allocator_connector_entry(node->connectors.entries[i]);

    ConnectorShape connector_shape = ConnectorShape(connector);

    draw_connector_handle_transform_trigger(connector);
    draw_connector_handle_transform_release(connector);
    connector_shape.draw();
  }

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && active_connector_handle)
    active_connector_handle = nullptr;
}

/**
   Detect when we hover one of the connectors touch points/ handles and move it
   with the mouse on click by updating the touch point position coordinate which
   will then affect the draw called upon.
 */
void Widget::CanvasShape::draw_connector_handle_transform_trigger(
    Connector *connector) {

  for (uint8_t i = 0; i < CONNECTOR_TOUCH_POINT_COUNT; i++) {
    ConnectorHandle *handle = &connector->handles[i];

    if (ImGui::IsMouseHoveringRect(
            ImVec2(vpx(handle->start[0]), vpy(handle->start[1])),
            ImVec2(vpx(handle->end[0]), vpy(handle->end[1])))) {

      ConnectorHandleShape(handle, ConnectorHandleSide_None).draw();

      if (ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
          !active_connector_handle) {
        active_connector_handle = handle;
      }
    }
  }
}

void Widget::CanvasShape::draw_connector_handle_transform_release(
    Connector *connector) {

  if (active_connector_handle) {

    ImVec2 mouse = vp_im2_scene(ImGui::GetIO().MousePos);
    connector_handle_set_position(active_connector_handle,
                                  (vec2){mouse.x, mouse.y});
    connector_compute_corners(connector);

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
        connect_system_connect_handle_to_frame(
            active_connector_handle, connector,
            node->frames[CanvasFrameState_Default].entries,
            node->frames[CanvasFrameState_Default].length) ==
            ConnectSystemStatus_Success)
      active_connector_handle = nullptr;
  }
}

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
