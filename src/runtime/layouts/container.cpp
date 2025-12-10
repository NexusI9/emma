#include "container.hpp"
#include "resources/theme.emma.h"
#include "runtime/layouts/navbar.hpp"
#include "runtime/manager/atlas.h"
#include "runtime/node/canvas.h"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/canvas/canvas.hpp"
#include "runtime/widgets/heatmap.hpp"
#include "runtime/widgets/utils.hpp"
#include "utils/input.h"

Layout::Container::Component::Component(Gui *gui, Canvas *canvas,
                                        Heatmap hm[HeatmapType_COUNT])
    : Core(gui, canvas), canvas(gui, canvas),
      toolbar(texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI)),
      heatmaps{
          {gui, &hm[0]},
          {gui, &hm[1]},
          {gui, &hm[2]},
          {gui, &hm[3]},
      },
      sidebar(gui, canvas),
      navbar(gui, canvas,
             {
                 .setter = set_octalysis_state,
                 .getter = get_octalysis_state,
                 .init_state = display_state_enabled(DisplayState_Octagon),
                 .user_data = this,
             },
             {
                 .setter = set_heatmap_state,
                 .getter = get_heatmap_state,
                 .init_state = display_state_enabled(DisplayState_Heatmap),
                 .user_data = this,
             }) {

  {
    // === Create heamap select shape ===
    heatmap_list_shape = UI::Frame();
    heatmap_list_shape.background_color =
        emma_im_color(ThemeEmmaColor_Surface_Lower);
    heatmap_list_shape.border_color =
        emma_im_color(ThemeEmmaColor_Border_Subtle_On_Dark);
    heatmap_list_shape.padding = gui_scale_im_vec2(gui, navbar.padding);
    heatmap_list_shape.border_radius = gui_scale(gui, navbar.bd_radius);
    heatmap_list_shape.position =
        ImVec2(gui_scale(gui, 1275), gui_scale(gui, 80));
    heatmap_list_shape.size = ImVec2(0, 0);
  }

  toolbar.add_callback(on_toolbar_update, this);
}

void Layout::Container::Component::draw() {

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

    navbar.update();
    toolbar.update();
    sidebar.update();

    canvas.draw();

    if (display_state_enabled(DisplayState_Heatmap)) {
      heatmaps[active_heatmap].draw();
      draw_heatmap_list();
    }

    navbar.render();
    toolbar.render();
    sidebar.render();
  }
  UI::FullScreenWindow().End();

  ImGui::Render();
  ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), gui->pass_encoder);

  gui_draw_swapchain_end(gui);
  gui_command_end(gui);

  viewport_update(&g_viewport_manager);
}

void Layout::Container::Component::draw_heatmap_list() {

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

void Layout::Container::draw_callback(Renderer *renderer, void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;
  container->draw();
}

void Layout::Container::set_octalysis_state(bool value, void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;

  if (value) {
    container->enable_display_state(
        Layout::Container::Component::DisplayState_Octagon);
  } else {
    container->disable_display_state(
        Layout::Container::Component::DisplayState_Octagon);
  }
}

void Layout::Container::set_heatmap_state(bool value, void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;

  if (value) {
    container->enable_display_state(
        Layout::Container::Component::DisplayState_Heatmap);
  } else {
    container->disable_display_state(
        Layout::Container::Component::DisplayState_Heatmap);
  }

  container->heatmap_state |=
      Layout::Container::Component::HeatmapState_RequireUpdate;
}

bool Layout::Container::get_octalysis_state(void *data) {
  return ((Layout::Container::Component *)data)
      ->display_state_enabled(
          Layout::Container::Component::DisplayState_Octagon);
}

bool Layout::Container::get_heatmap_state(void *data) {
  return ((Layout::Container::Component *)data)
      ->display_state_enabled(
          Layout::Container::Component::DisplayState_Heatmap);
}

void Layout::Container::on_toolbar_update(const uint8_t active, void *data) {
  Layout::Container::Component *container =
      (Layout::Container::Component *)data;
  container->update_canvas_mode((Widget::Canvas::Create::Mode)active);
  container->canvas_enable_state(
      Widget::Canvas::Component::State::State_FreezeCreationSession);
}

void Layout::Container::on_sidebar_tab_update(const uint8_t active,
                                              void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;
  container->update_canvas_mode((Widget::Canvas::Create::Mode)active);
  container->canvas_enable_state(
      Widget::Canvas::Component::State::State_FreezeCreationSession);
}
