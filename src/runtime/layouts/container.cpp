#include "container.hpp"
#include "runtime/layouts/nav_bar.hpp"
#include "runtime/manager/atlas.h"
#include "runtime/node/canvas.h"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/canvas.hpp"
#include "runtime/widgets/heatmap.hpp"

Layout::Container::Container(Gui *gui, Canvas *canvas,
                             Heatmap hm[HeatmapType_COUNT])
    : gui(gui), canvas_shape(gui, canvas),
      tool_bar(texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI)),
      heatmaps{&hm[0], &hm[1], &hm[2], &hm[3]},
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

  gui_command_begin(gui);
  gui_draw_update_io(gui);

  if (display_state_enabled(DisplayState_Heatmap) && heatmap_require_update) {
    heatmap_require_update = false;
    heatmaps[active_heatmap].compute_offline(gui->command_encoder);
  }

  gui_draw_swapchain_begin(gui);
  ImGui_ImplWGPU_NewFrame();
  ImGui::NewFrame();

  UI::FullScreenWindow().Begin("Root container");
  {
    canvas_shape.draw(display_state_enabled(DisplayState_Octagon));
    tool_bar.draw();

    if (display_state_enabled(DisplayState_Heatmap))
      heatmaps[active_heatmap].draw();

    nav_bar.draw();
  }
  UI::FullScreenWindow().End();

  ImGui::Render();
  ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), gui->pass_encoder);

  gui_draw_swapchain_end(gui);
  gui_command_end(gui);

  viewport_update(&g_viewport_manager);
}

void container_draw_callback(Renderer *renderer, void *data) {

  Layout::Container *container = (Layout::Container *)data;
  container->draw();
}

void container_set_octalysis_state(bool value, void *data) {

  Layout::Container *container = (Layout::Container *)data;

  if (value) {
    container->enable_display_state(Layout::Container::DisplayState_Octagon);
  } else {
    container->disable_display_state(Layout::Container::DisplayState_Octagon);
  }
}

void container_set_heatmap_state(bool value, void *data) {

  Layout::Container *container = (Layout::Container *)data;

  if (value) {
    container->enable_display_state(Layout::Container::DisplayState_Heatmap);
  } else {
    container->disable_display_state(Layout::Container::DisplayState_Heatmap);
  }

  container->heatmap_require_update = true;
}

bool container_get_octalysis_state(void *data) {
  return ((Layout::Container *)data)
      ->display_state_enabled(Layout::Container::DisplayState_Octagon);
}

bool container_get_heatmap_state(void *data) {
  return ((Layout::Container *)data)
      ->display_state_enabled(Layout::Container::DisplayState_Heatmap);
}
