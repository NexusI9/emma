#include "nav_bar.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/node/canvas.h"

Layout::NavBar::NavBar(Gui *gui, Canvas *canvas, NavBarSwitchConfig octa_config,
                       NavBarSwitchConfig heatmap_config)
    : canvas(canvas), gui(gui), oct_switch(octa_config.init_state),
      heat_switch(heatmap_config.init_state), octalysis_config(octa_config),
      heatmap_config(heatmap_config) {

  // TODO COLOR
  {
    // === Setup left panel ===
    left_panel.position = gui_scale_im_vec2(gui, margin);
    left_panel.background_color = bg_color;
    left_panel.border_color = bd_color;
    left_panel.padding = gui_scale_im_vec2(gui, padding);
    left_panel.border_radius = gui_scale(gui, bd_radius);
    left_panel.size = ImVec2(gui_scale(gui, 180), gui_scale(gui, height));
  }

  {
    // === Setup right panel ===
    right_panel.position =
        ImVec2(gui_scale(gui, 1160), gui_scale(gui, margin.y));
    right_panel.background_color = bg_color;
    right_panel.border_color = bd_color;
    right_panel.padding = gui_scale_im_vec2(gui, padding);
    right_panel.border_radius = gui_scale(gui, bd_radius);
    right_panel.size = ImVec2(0, gui_scale(gui, height));
  }

  {
    // === Setup switches ===
    UI::Switch *switches[] = {&oct_switch, &heat_switch};
    static const int switch_count = sizeof(switches) / sizeof(switches[0]);
    for (uint8_t i = 0; i < switch_count; i++) {
      switches[i]->width = gui_scale(gui, 48);
      switches[i]->height = gui_scale(gui, 24);
      switches[i]->border_radius = gui_scale(gui, 6.0f);
      switches[i]->dot_size = gui_scale(gui, 18.0f);
      switches[i]->dot_border_radius = gui_scale(gui, 3.0f);
      switches[i]->bg_color_default = ImColor(44, 44, 44, 255);
      switches[i]->bg_color_active = ImColor(2, 45, 105, 255);
      switches[i]->border_color_default = ImColor(123, 123, 123, 255);
      switches[i]->border_color_active = ImColor(152, 217, 255, 255);
      switches[i]->dot_color_default = ImColor(123, 123, 123, 255);
      switches[i]->dot_color_active = ImColor(12, 140, 233, 255);
    }
  }
}

void Layout::NavBar::draw() {

  left_panel.Begin("Left Panel");
  {
    const float base_y = ImGui::GetCursorPosY();
    ImGui::SetCursorPosY(base_y + gui_scale(gui, 3));
    ImGui::Text("EMMA [v0.15 - alpha]");
    ImGui::SetCursorPosX(gui_scale(gui, margin.x));
    ImGui::SetWindowFontScale(0.8f);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::Text("Emotional Mapper");
    ImGui::PopStyleColor();
    ImGui::SetWindowFontScale(1.0f);
  }
  left_panel.End();

  right_panel.Begin("Right Panel");
  {
    const float base_y = ImGui::GetCursorPosY();
    draw_switch(&oct_switch, "##oct_switch", "Octalysis", &octalysis_config,
                base_y);
    ImGui::SameLine(0.0f, gui_scale(gui, 16));
    draw_switch(&heat_switch, "##heat_switch", "Heatmap", &heatmap_config,
                base_y);
  }
  right_panel.End();
}

void Layout::NavBar::draw_switch(UI::Switch *component, const char *id,
                                 const char *label, NavBarSwitchConfig *config,
                                 const float base_y) {

  static const float switch_padding_top = 11.0f;
  static const float text_padding_top = 20.0f;
  static const float text_gap = 8.0f;

  {
    ImGui::SetCursorPosY(base_y + text_padding_top);
    ImGui::Text("%s", label);

    ImGui::SameLine(0.0f, gui_scale(gui, text_gap));

    ImGui::SetCursorPosY(base_y + switch_padding_top);

    bool switch_state = component->Draw(id);

    if (switch_state && !config->getter(config->user_data)) {

      config->setter(true, config->user_data);

    } else if (!switch_state && config->getter(config->user_data)) {

      config->setter(false, config->user_data);
    }
  }
}
