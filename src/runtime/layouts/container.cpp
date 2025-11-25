#include "container.hpp"
#include "runtime/layouts/nav_bar.hpp"
#include "runtime/manager/atlas.h"
#include "runtime/widgets/canvas.hpp"

Layout::Container::Container(Gui *gui, Canvas *canvas)
    : gui(gui), canvas_shape(gui, canvas),
      tool_bar(texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI)),
      nav_bar(gui, canvas) {

  canvas_shape.sync_shapes();
}

void Layout::Container::draw() {

  gui_draw_begin(gui);
  gui_draw_update_io(gui);
  {
    ImGui_ImplWGPU_NewFrame();
    ImGui::NewFrame();

    ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->Pos);
    ImGui::SetNextWindowSize(vp->Size);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Root Container", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

    {
      canvas_shape.draw();
      nav_bar.draw();
      tool_bar.draw();
    }

    ImGui::End();
    ImGui::PopStyleVar(2);

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
