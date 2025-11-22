#include "canvas.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_wgpu.h"
#include "nkengine/include/gui.hpp"
#include "runtime/allocator.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/canvas.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/widgets/canvas.hpp"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/grid_background.hpp"
#include "runtime/widgets/octagon.hpp"
#include "runtime/widgets/transform_box.hpp"
#include <imgui/imconfig.h>
#include <imgui/imgui_impl_wgpu.h>

Widget::CanvasShape::CanvasShape(Gui *gui, Canvas *canvas) {

  this->gui = gui;
  this->node = canvas;
  transform_box = TransformBox(gui);

  transform_box.update_bound(ImVec2(20, 20), ImVec2(900, 300));

  HTMLEventWheel event = {
      .callback = canvas_shape_wheel_callback,
      .destructor = NULL,
      .data = this,
      .size = 0,
  };
  html_event_add_wheel(&event);
}

void Widget::CanvasShape::draw_frame(FrameShape *shape) {

  Frame *frame = shape->get_node();
  shape->draw();

  // add to selection
  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {

    transform_box.session_set_blank_click();

    for (uint8_t i = 0; i < BOUNDBOX_FRAME_RECT_COUNT; i++)
      if (ImGui::IsMouseHoveringRect(im_vec2(shape->boundbox[i].p0),
                                     im_vec2(shape->boundbox[i].p1))) {

        transform_frame_data[i].frame = shape;
        transform_frame_data[i].canvas = node;

        TransformBoxObjectDescriptor object = {
            .handle = &transform_frame_data[i],
            .get_position = canvas_shape_get_frame_shape_position,
            .set_position = canvas_shape_set_frame_shape_position,
            .get_size = canvas_shape_get_frame_shape_size,
            .set_size = canvas_shape_set_frame_shape_size,
        };

        if (input_key(INPUT_KEY_CAP) == false)
          transform_box.empty_objects();

        transform_box.session_set_hit();
        transform_box.toggle_object(&object);
        transform_box.update_bound_from_selection();
      }
  }
}

void Widget::CanvasShape::draw() {

  gui_draw_begin(gui);
  gui_draw_update_io(gui);
  {
    ImGui_ImplWGPU_NewFrame();
    ImGui::NewFrame();
    {
      draw_grid_background();

      ImGuiViewport *vp = ImGui::GetMainViewport();

      // Push fullscreen position + size
      ImGui::SetNextWindowPos(vp->Pos);
      ImGui::SetNextWindowSize(vp->Size);

      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

      ImGui::Begin("Canvas", NULL,
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                       ImGuiWindowFlags_NoScrollWithMouse |
                       ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoBackground |
                       ImGuiWindowFlags_NoBringToFrontOnFocus);

      for (size_t i = 0; i < node->frames.length; i++)
        draw_frame(&frame_shapes[i]);

      for (size_t i = 0; i < node->octagons.length; i++) {
        const alloc_id id = node->octagons.entries[i];
        OctagonShape(allocator_octagon_entry(id)).draw();
      }

      transform_box.session_end();
      if (transform_box.objects_count() > 0)
        transform_box.draw();

      ImGui::End();
      ImGui::PopStyleVar(2);
    }
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), gui->pass_encoder);
  }
  gui_draw_end(gui);

  viewport_update(&g_viewport_manager);
}

/**
   Sync up the canvas data and generate shapes out of those.
 */
void Widget::CanvasShape::sync_shapes() {
  for (size_t i = 0; i < node->frames.length; i++) {
    const alloc_id id = node->frames.entries[i];
    Frame *frame = allocator_frame_entry(id);
    frame_shapes[i] = FrameShape(frame);
    boundbox_frame_update(frame_shapes[i].boundbox, frame->position,
                          frame->end_point, FRAME_SHAPE_BOUNDBOX_THICKNESS);
  }
}

void Widget::CanvasShape::sync_boundboxes() {

  for (size_t i = 0; i < node->frames.length; i++) {
    const alloc_id id = node->frames.entries[i];
    Frame *frame = allocator_frame_entry(id);
    boundbox_frame_update(frame_shapes[i].boundbox, frame->position,
                          frame->end_point, FRAME_SHAPE_BOUNDBOX_THICKNESS);
  }
}

/**
   On wheel change we need to update each objects boundbox.
   Another solution could be integrate a system of "require_change" triggered by
   the viewport, but we'll use the html event based solution for now as it comes
   builtin with the engine.
 */
bool Widget::canvas_shape_wheel_callback(int type,
                                         const EmscriptenWheelEvent *event,
                                         void *data) {

  CanvasShape *canvas = (CanvasShape *)data;
  canvas->sync_boundboxes();

  return EM_FALSE;
}

void Widget::canvas_shape_set_frame_shape_position(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame->get_node();

  canvas_set_frame_position(frame_data->canvas, frame,
                            (vec2){value.x, value.y});

  boundbox_frame_update(frame_data->frame->boundbox, frame->position,
                        frame->end_point, FRAME_SHAPE_BOUNDBOX_THICKNESS);
}

void Widget::canvas_shape_get_frame_shape_position(void *data, ImVec2 &value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame->get_node();

  value = im_vec2(frame->position);
}

void Widget::canvas_shape_set_frame_shape_size(void *data, ImVec2 value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame->get_node();

  canvas_set_frame_size(frame_data->canvas, frame, (vec2){value.x, value.y});

  boundbox_frame_update(frame_data->frame->boundbox, frame->position,
                        frame->end_point, FRAME_SHAPE_BOUNDBOX_THICKNESS);
}

void Widget::canvas_shape_get_frame_shape_size(void *data, ImVec2 &value) {

  CanvasTransformFrameData *frame_data = (CanvasTransformFrameData *)data;

  Frame *frame = frame_data->frame->get_node();

  value = im_vec2(frame->size);
}
