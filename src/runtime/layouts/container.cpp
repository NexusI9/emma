#include "container.hpp"
#include "runtime/layouts/nav_bar.hpp"
#include "runtime/manager/atlas.h"
#include "runtime/node/canvas.h"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/canvas.hpp"
#include "runtime/widgets/heatmap.hpp"
#include "utils/input.h"

Layout::Container::Container(Gui *gui, Canvas *canvas,
                             Heatmap hm[HeatmapType_COUNT])
    : gui(gui), canvas_shape(gui, canvas),
      tool_bar(texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI)),
      heatmaps{
          {gui, &hm[0]},
          {gui, &hm[1]},
          {gui, &hm[2]},
          {gui, &hm[3]},
      },
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

  {
    // === Create heamap select shape ===
    heatmap_list_shape = UI::Frame();
    heatmap_list_shape.background_color = nav_bar.bg_color;
    heatmap_list_shape.border_color = nav_bar.bd_color;
    heatmap_list_shape.padding = gui_scale_im_vec2(gui, nav_bar.padding);
    heatmap_list_shape.border_radius = gui_scale(gui, nav_bar.bd_radius);
    heatmap_list_shape.position =
        ImVec2(gui_scale(gui, 1275), gui_scale(gui, 80));
    heatmap_list_shape.size = ImVec2(0, 0);
  }
}

void Layout::Container::draw() {

  gui_command_begin(gui);
  gui_draw_update_io(gui);

  if (display_state_enabled(DisplayState_Heatmap)) {

    if ((HeatmapState_WheelMove & heatmap_state) && !input_wheel_moving())
      heatmap_state |= HeatmapState_RequireUpdate;

    if (input_wheel_moving())
      heatmap_state |= HeatmapState_WheelMove;
    else
      heatmap_state &= ~HeatmapState_WheelMove;

    if (HeatmapState_RequireUpdate & heatmap_state) {
      heatmap_state &= ~HeatmapState_RequireUpdate;
      heatmaps[active_heatmap].compute_offline(gui->command_encoder);
    }
  }

  gui_draw_swapchain_begin(gui);
  ImGui_ImplWGPU_NewFrame();
  ImGui::NewFrame();

  UI::FullScreenWindow().Begin("Root container");
  {
    canvas_shape.draw(display_state_enabled(DisplayState_Octagon));
    tool_bar.draw();

    if (display_state_enabled(DisplayState_Heatmap)) {
      heatmaps[active_heatmap].draw();
      draw_heatmap_list();
    }

    nav_bar.draw();
  }
  UI::FullScreenWindow().End();

  ImGui::Render();
  ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), gui->pass_encoder);

  gui_draw_swapchain_end(gui);
  gui_command_end(gui);

  viewport_update(&g_viewport_manager);
}

void Layout::Container::draw_heatmap_list() {

  heatmap_list_shape.Begin("##Heatmap list");

  ImGui::Text("Factors:");
  ImGui::Dummy(ImVec2(gui_scale(gui, 3.0f), gui_scale(gui, 3.0f)));
  for (int i = 0; i < HeatmapType_COUNT; i++) {
    if (ImGui::RadioButton(heatmaps[i].get_label(), active_heatmap == i)) {
      active_heatmap = (HeatmapType)i;
      heatmap_state |= HeatmapState_RequireUpdate;
    }
    ImGui::Dummy(ImVec2(gui_scale(gui, 3.0f), gui_scale(gui, 3.0f)));
  }
  heatmap_list_shape.End();
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

  container->heatmap_state |= Layout::Container::HeatmapState_RequireUpdate;
}

bool container_get_octalysis_state(void *data) {
  return ((Layout::Container *)data)
      ->display_state_enabled(Layout::Container::DisplayState_Octagon);
}

bool container_get_heatmap_state(void *data) {
  return ((Layout::Container *)data)
      ->display_state_enabled(Layout::Container::DisplayState_Heatmap);
}
