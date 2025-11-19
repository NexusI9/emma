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
  
}

void Widget::CanvasShape::draw() {

  gui_draw_begin(gui);
  gui_draw_update_io(gui);
  {
    ImGui_ImplWGPU_NewFrame();
    ImGui::NewFrame();
    {
      draw_grid_background();

      for (size_t i = 0; i < node->frames.length; i++) {
        const alloc_id id = node->frames.entries[i];
        FrameShape(allocator_frame_entry(id)).draw();
      }

      for (size_t i = 0; i < node->octagons.length; i++) {
        const alloc_id id = node->octagons.entries[i];
        OctagonShape(allocator_octagon_entry(id)).draw();
      }
    }
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), gui->pass_encoder);
  }
  gui_draw_end(gui);
}
