#include "canvas.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_wgpu.h"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/allocator.h"
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
#include <imgui/imconfig.h>
#include <imgui/imgui_impl_wgpu.h>

Widget::CanvasShape::CanvasShape(Gui *gui, Canvas *canvas)
    : grid_background("textures/dot-pattern.png", TextureResolution_64),
      transform_box(gui) {

  this->gui = gui;
  this->node = canvas;

  transform_box.update_bound(ImVec2(20, 20), ImVec2(900, 300));
}

void Widget::CanvasShape::draw_frame(Frame *frame,
                                     const TransformBoxDraw transform_type,
                                     const uint8_t boundboxes_count) {
  FrameShape(frame).draw();

  // add to selection
  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {

    transform_box.session_set_blank_click();

    for (uint8_t i = 0; i < boundboxes_count; i++)
      if (ImGui::IsMouseHoveringRect(
              ImVec2(vpx(frame->boundbox.entries[i].p0[0]),
                     vpy(frame->boundbox.entries[i].p0[1])),
              ImVec2(vpx(frame->boundbox.entries[i].p1[0]),
                     vpy(frame->boundbox.entries[i].p1[1])))) {

        // register the frame for transform callbacks
        transform_frame_data[i].frame = frame;
        transform_frame_data[i].canvas = node;

        TransformBoxObjectDescriptor object = {
            .handle = &transform_frame_data[i],
            .get_position = canvas_shape_get_frame_shape_position,
            .set_position = canvas_shape_set_frame_shape_position,
            .get_size = canvas_shape_get_frame_shape_size,
            .set_size = canvas_shape_set_frame_shape_size,
        };

        transform_box.session_set_hit();

        TransformBoxObject *found_obj =
            transform_box.find_object(object.handle, NULL);

        // remove object
        if (found_obj) {

          canvas_unregister_frame_state(node, frame, CanvasFrameState_Selected);
          transform_box.remove_object(found_obj->handle, NULL);

          // add object
        } else {

          if (input_key(INPUT_KEY_CAP) == false) {
            transform_box.empty_objects();
            canvas_empty_frame_state(node, CanvasFrameState_Selected);
          }

          canvas_register_frame_state(node, frame, CanvasFrameState_Selected);
          transform_box.add_object(&object);
        }

        transform_box.update_bound_from_selection();
      }
  }
}

void Widget::CanvasShape::draw_frame_handle_connectors(Frame *frame,
                                                       const int side) {

  for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++) {

    // TODO: maybe make a dedicated draw function for side to prevent branching
    if ((__builtin_ctz(side) & i) == 0)
      continue;

    ConnectorHandle *handle =
        allocator_connector_handle_entry(frame->connector_handle_id[i]);
    ConnectorHandleShape(handle).draw();
  }
}

void Widget::CanvasShape::draw(bool show_octagon) {

  grid_background.draw_texture(gui->pass_encoder);

  // === frames ===
  size_t i;
  for (i = 0; i < node->frames[CanvasFrameState_Default].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Default].entries[i]);
    draw_frame(frame, TransformBoxDraw_All, BOUNDBOX_FRAME_RECT_COUNT);
  }

  if (show_octagon)
    for (i = 0; i < node->frames[CanvasFrameState_Octagon].length; i++) {
      Frame *frame = allocator_frame_entry(
          node->frames[CanvasFrameState_Octagon].entries[i]);

      OctagonShape(allocator_octagon_entry(frame->octagon_id)).draw();
    }

  for (i = 0; i < node->frames[CanvasFrameState_Selected].length; i++) {
    Frame *frame = allocator_frame_entry(
        node->frames[CanvasFrameState_Selected].entries[i]);

    draw_frame_handle_connectors(frame, ConnectorHandleSide_Left |
                                            ConnectorHandleSide_Right);
  }

  // === modules ===
  for (i = 0; i < node->modules[CanvasModuleState_Default].length; i++) {
    Frame *module = allocator_frame_entry(
        node->modules[CanvasModuleState_Default].entries[i]);
    ModuleShape(module).draw();
  }

  // === connectors ===
  for (i = 0; i < node->connectors.length; i++) {
    Connector *connector =
        allocator_connector_entry(node->connectors.entries[i]);
    ConnectorShape(connector).draw();
  }

  // === transform box ===
  if (transform_box.session_end() == TransformBoxStatus_ClearSelection)
    canvas_empty_frame_state(node, CanvasFrameState_Selected);

  if (transform_box.objects_count() > 0)
    transform_box.draw(TransformBoxDraw_All);
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
