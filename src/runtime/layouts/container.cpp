#include "container.hpp"
#include "runtime/manager/atlas.h"
#include "runtime/widgets/canvas.hpp"

Layout::Container::Container(Gui *gui, Canvas *canvas)
    : gui(gui), canvas_shape(gui, canvas),
      tool_bar(texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI)) {

  canvas_shape.sync_shapes();
}

void Layout::Container::draw() {

  gui_draw_begin(gui);
  gui_draw_update_io(gui);
  {
    ImGui_ImplWGPU_NewFrame();
    ImGui::NewFrame();
    {
      canvas_shape.draw();
      tool_bar.draw();
    }
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), gui->pass_encoder);
  }
  gui_draw_end(gui);

  viewport_update(&g_viewport_manager);
}

void container_draw_callback(Renderer *renderer, void *data) {

  Layout::Container *container = (Layout::Container *)data;
  container->draw();
}
