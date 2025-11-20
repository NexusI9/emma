#include "canvas.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_wgpu.h"
#include "nkengine/include/gui.hpp"
#include "runtime/allocator.h"
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

  // DEBUG
  transform_box.update_bound(ImVec2(20, 20), ImVec2(900, 300));
}

void Widget::CanvasShape::draw_frame(FrameShape *shape) {

  Frame *frame = shape->get_node();
  shape->draw();

  for (uint8_t i = 0; i < BOUNDBOX_FRAME_RECT_COUNT; i++)
    // add to selection
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {

      if (ImGui::IsMouseHoveringRect(im_vec2(shape->boundbox[i].p0),
                                     im_vec2(shape->boundbox[i].p1))) {

        TransformBoxObjectDescriptor object = {
            .handle = shape,
            .get_position = frame_shape_get_position,
            .set_position = frame_shape_set_position,
            .get_size = frame_shape_get_size,
            .set_size = frame_shape_set_size,
        };

        transform_box.toggle_object(&object);
        transform_box.update_bound_from_selection();

      } else {
        transform_box.empty_objects();
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

      if (transform_box.objects_count())
        transform_box.draw();

      ImGui::End();
      ImGui::PopStyleVar(2);
    }
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), gui->pass_encoder);
  }
  gui_draw_end(gui);
}

/**
   Sync up the canvas data and generate shapes out of those.
 */
void Widget::CanvasShape::update_frame_shapes() {
  for (size_t i = 0; i < node->frames.length; i++) {
    const alloc_id id = node->frames.entries[i];
    Frame *frame = allocator_frame_entry(id);
    frame_shapes[i] = FrameShape(frame);
    boundbox_frame_update(frame_shapes[i].boundbox, frame->position,
                          frame->end_point, FRAME_SHAPE_BOUNDBOX_THICKNESS);
  }
}
