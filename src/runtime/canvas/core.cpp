#include "core.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_wgpu.h"
#include "nkengine/include/gui.hpp"
#include "runtime/allocator.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/grid_background.hpp"
#include "runtime/widgets/octagon.hpp"

Frame *canvas_create_frame(Canvas *canvas) {

  const size_t capacity = allocator_frame_capacity();

  if (canvas->frames.length == capacity)
    return NULL;

  Frame *frame = new_frame();

  if (frame) {
    allocator_id_list_push(canvas->frames.entries, capacity,
                           &canvas->frames.length, frame->id);

    FrameDescriptor frame_desc = {
        .background = {1.0f, 1.0f, 1.0f, 1.0f},
        .position = {200, 200},
        .size = {360, 700},
    };

    frame_create(frame, &frame_desc);
  }

  return frame;
}

Octagon *canvas_create_octagon(Canvas *canvas) {

  const size_t capacity = allocator_octagon_capacity();

  if (canvas->frames.length == capacity)
    return NULL;

  Octagon *oct = new_octagon();

  if (oct) {
    allocator_id_list_push(canvas->octagons.entries, capacity,
                           &canvas->octagons.length, oct->id);

    OctagonDescriptor oct_desc = {
        .inner_color = {0.1f, 0.1, 0.1f, 1.0f},
        .outer_color = {1.0f, 1.0f, 1.0f, 1.0f},
        .inner_radius = 0.6f,
        .label = "Octagon",
        .position =
            {
                context_width() / 2.0f * (float)context_dpi(),
                context_height() / 2.0f * (float)context_dpi(),
            },
        .scale = 300.0f,
    };

    octagon_create(oct, &oct_desc);
  }

  return oct;
}

void canvas_draw_callback(Renderer *renderer, void *data) {

  CanvasDrawData *draw_data = (CanvasDrawData *)data;
  Gui *gui = draw_data->gui;
  Canvas *canvas = draw_data->canvas;

  gui_draw_begin(gui);
  gui_draw_update_io(gui);
  {
    ImGui_ImplWGPU_NewFrame();
    ImGui::NewFrame();
    {
      draw_grid_background();

      for (size_t i = 0; i < canvas->frames.length; i++) {
        const alloc_id id = canvas->frames.entries[i];
        Widget::FrameShape(allocator_frame_entry(id)).draw();
      }

      for (size_t i = 0; i < canvas->octagons.length; i++) {
        const alloc_id id = canvas->octagons.entries[i];
        Widget::OctagonShape(allocator_octagon_entry(id)).draw();
      }
    }
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), gui->pass_encoder);
  }
  gui_draw_end(gui);
}
