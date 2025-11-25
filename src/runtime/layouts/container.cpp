#include "container.hpp"
#include "runtime/layouts/nav_bar.hpp"
#include "runtime/manager/atlas.h"
#include "runtime/node/canvas.h"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/canvas.hpp"
#include "runtime/widgets/heatmap.hpp"

Layout::Container::Container(Gui *gui, Canvas *canvas, Heatmap *heatmap)
    : gui(gui), canvas_shape(gui, canvas),
      tool_bar(texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI)),
      heatmap(heatmap),
      nav_bar(gui, canvas,
              {
                  .setter = container_set_octalysis_state,
                  .getter = container_get_octalysis_state,
                  .init_state = display_state_enabled(DisplayState_Octagon),
                  .user_data = this,
              },
              {
                  .setter = container_set_heatmap_state,
                  .getter = container_get_heatmap_state,
                  .init_state = display_state_enabled(DisplayState_Heatmap),
                  .user_data = this,
              }) {

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
      canvas_shape.draw(display_state_enabled(DisplayState_Octagon));
      tool_bar.draw();

      if (display_state_enabled(DisplayState_Heatmap))
        heatmap.draw();

      nav_bar.draw();
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

void container_set_octalysis_state(bool value, void *data) {

  if (value)
    ((Layout::Container *)data)
        ->enable_display_state(Layout::Container::DisplayState_Octagon);
  else
    ((Layout::Container *)data)
        ->disable_display_state(Layout::Container::DisplayState_Octagon);
}

void container_set_heatmap_state(bool value, void *data) {

  if (value)
    ((Layout::Container *)data)
        ->enable_display_state(Layout::Container::DisplayState_Heatmap);
  else
    ((Layout::Container *)data)
        ->disable_display_state(Layout::Container::DisplayState_Heatmap);
}

bool container_get_octalysis_state(void *data) {
  return ((Layout::Container *)data)
      ->display_state_enabled(Layout::Container::DisplayState_Octagon);
}

bool container_get_heatmap_state(void *data) {
  return ((Layout::Container *)data)
      ->display_state_enabled(Layout::Container::DisplayState_Heatmap);
}
