#include "navbar.hpp"
#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/node/canvas.h"
#include "runtime/widgets/utils.hpp"

Layout::NavBar::Component::Component(Gui *gui, Canvas *canvas,
                                     SwitchConfig octa_config,
                                     SwitchConfig heatmap_config)
    : Core(gui, canvas),
      switches{
          {
              .component = {octa_config.init_state},
              .cached_position = ImVec2(0, 0),
          },
          {
              .component = {heatmap_config.init_state},
              .cached_position = ImVec2(0, 0),
          },
      },
      octalysis_config(octa_config), heatmap_config(heatmap_config) {

  {
    // === Setup main panel ===

    // clang-format off
    main_panel.position = ImVec2(gui_scale(gui, margin.x), gui_scale(gui, margin.y));
    main_panel.background_color =  emma_im_color(ThemeEmmaColor_Surface_Transparent);
    main_panel.border_color = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
    main_panel.padding = gui_scale_im_vec2(gui, padding);
    main_panel.border_radius = gui_scale(gui, bd_radius);
    main_panel.size = ImVec2(0, gui_scale(gui, height));
    // clang-format on
  }

  {
    // === Setup switches ===
    for (uint8_t i = 0; i < DisplaySwitch_COUNT; i++) {

      // clang-format off
      switches[i].component.width = gui_scale(gui, 48);
      switches[i].component.height = gui_scale(gui, 24);
      switches[i].component.border_radius = gui_scale(gui, 6.0f);
      switches[i].component.dot_size = gui_scale(gui, 18.0f);
      switches[i].component.dot_border_radius = gui_scale(gui, 3.0f);
      switches[i].component.bg_color_default = emma_im_color(ThemeEmmaColor_Surface_Lower);
      switches[i].component.bg_color_active = emma_im_color(ThemeEmmaColor_Background_Brand_Strong);
      switches[i].component.border_color_default = emma_im_color(ThemeEmmaColor_Border_Subtle_On_Dark);
      switches[i].component.border_color_active = emma_im_color(ThemeEmmaColor_Border_Brand_Subtle);
      switches[i].component.dot_color_default = emma_im_color(ThemeEmmaColor_Surface_Base);
      switches[i].component.dot_color_active = emma_im_color(ThemeEmmaColor_Background_Brand_Base);
      // clang-format on
    }
  }

  set_project_name("Gamification Userflow - V1");
}

void Layout::NavBar::Component::update() {
  update_switch(DisplaySwitch_Octagon, "##oct_switch", &octalysis_config);
  update_switch(DisplaySwitch_Heatmap, "##heat_switch", &heatmap_config);
}

void Layout::NavBar::Component::render() {

  main_panel.Begin("Main Panel");

  const float base_y = ImGui::GetCursorPosY();

  {
    ImGui::SetCursorPosY(base_y + gui_scale(gui, 3));
    ImGui::Text("EMMA [v0.15 - alpha]");
    ImGui::SetCursorPosX(gui_scale(gui, margin.x));
    ImGui::SetWindowFontScale(0.8f);
    ImGui::PushStyleColor(
        ImGuiCol_Text,
        (ImVec4)emma_im_color(ThemeEmmaColor_Text_Subtle_On_Dark));
    ImGui::Text("Emotional Mapper");
    ImGui::PopStyleColor();
    ImGui::SetWindowFontScale(1.0f);
  }

  ImVec2 title_size = ImGui::CalcTextSize(project_name);
  ImVec2 project_name_pos =
      ImVec2(ImGui::GetMainViewport()->Size.x / 2 - title_size.x / 2,
             base_y + gui_scale(gui, 12));
  ImGui::SetCursorPos(project_name_pos);
  ImGui::Text("%s", project_name);

  {
    // display sitches
    ImGui::SetCursorPos(ImVec2(gui_scale(gui, 1160), base_y));
    draw_switch(DisplaySwitch_Octagon, "Octalysis", base_y);
    ImGui::SameLine(0.0f, gui_scale(gui, 16));
    draw_switch(DisplaySwitch_Heatmap, "Heatmap", base_y);
  }
  main_panel.End();
}

/**
   Update the switch state without drawing it. We need to split the logic from
   the rendering cause the switch callback need to happened before the canvas
   draw.
 */
void Layout::NavBar::Component::update_switch(const DisplaySwitch type,
                                              const char *id,
                                              SwitchConfig *config) {

  UI::Switch *component = &switches[type].component;

  ImGui::SetCursorPos(switches[type].cached_position);

  bool switch_state = component->Update(id);

  if (switch_state && !config->getter(config->user_data))
    config->setter(true, config->user_data);
  else if (!switch_state && config->getter(config->user_data))
    config->setter(false, config->user_data);
}

void Layout::NavBar::Component::draw_switch(const DisplaySwitch type,
                                            const char *label,
                                            const float base_y) {

  static const float switch_padding_top = 11.0f;
  static const float text_padding_top = 20.0f;
  static const float text_gap = 8.0f;

  UI::Switch *component = &switches[type].component;

  ImGui::SetCursorPosY(base_y + text_padding_top);
  ImGui::Text("%s", label);

  ImGui::SameLine(0.0f, gui_scale(gui, text_gap));
  ImGui::SetCursorPosY(base_y + switch_padding_top);

  // cache position for update
  //(not ideal but does the job for the scarch case)
  if (switches[type].cached_position[0] == 0)
    switches[type].cached_position = ImGui::GetCursorPos();

  component->Render();
}
