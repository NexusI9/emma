#include "canvas.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_wgpu.h"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/canvas.h"
#include "runtime/node/connector.h"
#include "runtime/node/connector_handle.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/widgets/canvas.hpp"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/grid_background.hpp"
#include "runtime/widgets/module.hpp"
#include "runtime/widgets/octagon.hpp"
#include "runtime/widgets/transform_box.hpp"
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

    fm_conf->transform_mode = TransformBoxMode_All,
    fm_conf->get_position = canvas_shape_get_frame_shape_position,
    fm_conf->set_position = canvas_shape_set_frame_shape_position,
    fm_conf->get_size = canvas_shape_get_frame_shape_size,
    fm_conf->set_size = canvas_shape_set_frame_shape_size,
    fm_conf->selection_list = {
        .entries = node->frames[CanvasFrameState_Selected].entries,
        .length = &node->frames[CanvasFrameState_Selected].length,
    };
  }

  {
    // Module transform config
    CanvasTransformConfiguration *md_conf =
        &transform_configuration[CanvasTransformConfigurationType_Module];

    md_conf->transform_mode = TransformBoxMode_Move,
    md_conf->get_position = canvas_shape_get_frame_shape_position,
    md_conf->set_position = canvas_shape_set_frame_shape_position,
    md_conf->get_size = canvas_shape_get_frame_shape_size,
    md_conf->set_size = canvas_shape_set_frame_shape_size,
    md_conf->selection_list = {
        .entries = node->modules[CanvasModuleState_Selected].entries,
        .length = &node->modules[CanvasModuleState_Selected].length,
    };
  }
}

/**
   Handle the boundbox interaction along with the transformation for frames
   and modules.
 */
void Widget::CanvasShape::draw_frame_transform(
    Frame *frame, const CanvasTransformConfiguration *conf) {

  // DEBUG
  for (uint8_t i = 0; i < frame->boundbox.count; i++) {
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(vpx(frame->boundbox.entries[i].p0[0]),
               vpy(frame->boundbox.entries[i].p0[1])),
        ImVec2(vpx(frame->boundbox.entries[i].p1[0]),
               vpy(frame->boundbox.entries[i].p1[1])),
        ImColor(255, 0, 0, 255));
  }

  // add to selection
  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {

    for (uint8_t i = 0; i < frame->boundbox.count; i++)
      if (ImGui::IsMouseHoveringRect(
              ImVec2(vpx(frame->boundbox.entries[i].p0[0]),
                     vpy(frame->boundbox.entries[i].p0[1])),
              ImVec2(vpx(frame->boundbox.entries[i].p1[0]),
                     vpy(frame->boundbox.entries[i].p1[1])))) {

        CanvasTransformFrameData data;
        data.frame = frame;
        data.canvas = node;

        // register the frame for transform callbacks
        if (stli_insert(transform_frame_data.entries, ALLOCATOR_MAX_FRAMES,
                        &transform_frame_data.count,
                        sizeof(CanvasTransformFrameData), &data,
                        "Canvas Transform Frame Data") !=
            StaticListStatus_Success)
          return;

        TransformBoxObjectDescriptor object = {
            .handle =
                &transform_frame_data.entries[transform_frame_data.count - 1],
            .get_position = conf->get_position,
            .set_position = conf->set_position,
            .get_size = conf->get_size,
            .set_size = conf->set_size,
        };

        transform_box.session_set_hit();

        TransformBoxObject *found_obj =
            transform_box.find_object(object.handle, NULL);

        // remove object
        if (found_obj) {
          // mark as unselected
          allocator_id_list_pop(conf->selection_list.entries,
                                conf->selection_list.length, frame->id);

          transform_box.remove_object(found_obj->handle, NULL);

          // remove it from the cached callback data
          for (size_t i = 0; i < transform_frame_data.count; i++)
            if (transform_frame_data.entries[i].frame == frame)
              stli_remove_at_index(transform_frame_data.entries,
                                   &transform_frame_data.count,
                                   sizeof(CanvasTransformFrameData), i);

          // add object
        } else {

          transform_box.mode = conf->transform_mode;

          // if not CAP input or if the configuration's Transform Mode is
          // different from the current one, we empty the selection.
          if (input_key(INPUT_KEY_CAP) == false ||
              transform_box.mode != conf->transform_mode) {

            transform_box.empty_objects();
            canvas_empty_frame_state(node, CanvasFrameState_Selected);
          }

          // mark as selected
          allocator_id_list_push(conf->selection_list.entries,
                                 ALLOCATOR_MAX_FRAMES,
                                 conf->selection_list.length, frame->id);

          transform_box.add_object(&object);
        }

        transform_box.update_bound_from_selection();
      }
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
    ConnectorHandleShape(handle).draw();
  }
}

void Widget::CanvasShape::draw(bool show_octagon) {

  grid_background.draw_texture(gui->pass_encoder);

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    transform_box.session_set_blank_click();

  // === Frames ===
  size_t i;
  for (i = 0; i < node->frames[CanvasFrameState_Default].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Default].entries[i]);

    FrameShape(frame).draw();

    draw_frame_transform(
        frame,
        &transform_configuration[CanvasTransformConfigurationType_Frame]);
  }

  // === Octagons ===
  if (show_octagon)
    for (i = 0; i < node->frames[CanvasFrameState_Octagon].length; i++) {
      Frame *frame = allocator_frame_entry(
          node->frames[CanvasFrameState_Octagon].entries[i]);

      OctagonShape(allocator_octagon_entry(frame->octagon_id)).draw();
    }

  // === Connectors Handles (On Select Only) ===
  for (i = 0; i < node->frames[CanvasFrameState_Selected].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Selected].entries[i]);
    draw_frame_handle_connectors(frame, ConnectorHandleSide_Left |
                                            ConnectorHandleSide_Right);
  }

  // === Modules ===
  for (i = 0; i < node->modules[CanvasModuleState_Default].length; i++) {
    Frame *module = allocator_frame_entry(
        node->modules[CanvasModuleState_Default].entries[i]);

    ModuleShape(module).draw();

    draw_frame_transform(
        module,
        &transform_configuration[CanvasTransformConfigurationType_Module]);
  }

  // === Connectors ===
  for (i = 0; i < node->connectors.length; i++) {
    Connector *connector =
        allocator_connector_entry(node->connectors.entries[i]);
    ConnectorShape(connector).draw();
  }

  // === Transform Box ===
  if (transform_box.session_end() == TransformBoxStatus_ClearSelection)
    canvas_empty_frame_state(node, CanvasFrameState_Selected);

  if (transform_box.objects_count() > 0)
    transform_box.draw();
}

void Widget::canvas_shape_set_frame_shape_position(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;
  canvas_set_frame_position(frame_data->canvas, frame,
                            (vec2){value.x, value.y});

  canvas_update_frame_connectors(frame_data->canvas, frame_data->frame);
}

void Widget::canvas_shape_get_frame_shape_position(void *data, ImVec2 &value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  const float *world_pos = frame_get_world_position(frame);
  value = ImVec2(world_pos[0], world_pos[1]);
}

void Widget::canvas_shape_set_frame_shape_size(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_frame_size(frame_data->canvas, frame, (vec2){value.x, value.y});
}

void Widget::canvas_shape_get_frame_shape_size(void *data, ImVec2 &value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame;

  value = im_vec2(frame->size);
}
