#include "sidebar.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"

Widget::SideBar::Component::Component(const char *label, Gui *gui,
                                      Canvas *canvas)
    : Widget(gui), Window(label),
      panel{
          {gui},
          {gui},
      },
      tabs{
          {gui, ui_sprite(UISprite_Sidebar_Icon_Module)},
          {gui, ui_sprite(UISprite_Sidebar_Icon_Persona)},
      },
      content{
          {gui},
          {gui},
          {gui},
          {gui},
      } {

  layout();
  content.modules.add_module_click_callback(on_module_click, this);
  content.personas.add_persona_click_callback(on_persona_click, this);
}

void Widget::SideBar::Component::layout() {

  panel[0].set_position(ImVec2(panel_margin_left, gui_scale(gui, 92)));
  panel[1].set_position(ImVec2(panel[0].p1.x, panel[0].p0.y));

  position = ImVec2(0, gui_scale(gui, 92));
  default_size = ImVec2(gui_scale(gui, 54), position.y + panel[0].init_size.y);
  size = default_size;

  bar_p1 = ImVec2(panel_margin_left, default_size.y);

  shadow_p0 = ImVec2(panel[0].p1.x, panel[0].p0.y);
  shadow_p1 = ImVec2(shadow_p0.x + gui_scale(gui, 32), default_size.y);

  static const ImVec2 buttons_base_position =
      ImVec2(tab_button_offset, gui_scale(gui, 112));
  static const int gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  float y = buttons_base_position.y;
  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    tabs[i].set_position(ImVec2(buttons_base_position.x, y));
    y += (i + 1) * (gap + tabs[i].height);
  }
}

void Widget::SideBar::Component::draw() {

  UI::DockedWindow().Begin(label, position, size);

  if (state == State_Level_1 || state == State_Level_2)
    draw_transparent_bar();

  if (active_tab >= 0) {
    panel[0].begin("Sidepanel Level 1");
    contents[active_tab]->draw();
    panel[0].end();
  }

  if (state == State_Level_2) {

    panel[1].begin("Sidepanel Level 2");
    draw_shadow_bar();
    sub_contents[active_tab]->draw();
    panel[1].end();

    draw_shadow_bar();
  }

  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    if (tabs[i].update()) {

      active_tab = (active_tab == i) ? -1 : i;
      set_state((active_tab > -1) ? State_Level_1 : State_Closed);

      update_window_width();
      update_tabs_position();

      content.modules.clear_selection();
      content.personas.clear_selection();

      for (uint8_t j = 0; j < tab_update_callbacks.count; j++)
        tab_update_callbacks.entries[j].callback(
            active_tab, tab_update_callbacks.entries[j].data);
    }

    tabs[i].render(active_tab == i);
  }

  UI::DockedWindow().End();
}

void Widget::SideBar::Component::update_tabs_position() {

  // update active/ inactive position
  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    if (active_tab == i) {
      tabs[i].set_position(ImVec2(-2, tabs[i].get_position().y));
    } else {
      tabs[i].set_position(ImVec2(tab_button_offset, tabs[i].get_position().y));
    }
  }
}

void Widget::SideBar::Component::draw_transparent_bar() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  // Transparent background
  dl->AddRectFilled(position, bar_p1,
                    emma_im_color(ThemeEmmaColor_Surface_Transparent));
}

void Widget::SideBar::Component::draw_shadow_bar() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  dl->AddRectFilledMultiColor(shadow_p0, shadow_p1, ImColor(0, 0, 0, 56),
                              ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0),
                              ImColor(0, 0, 0, 56));
}

void Widget::SideBar::on_module_click(const ModuleType type, bool active,
                                      void *data) {

  Component *sidebar = (Component *)data;

  if (active) {
    sidebar->content.modules_editor.set_module(type);
    sidebar->set_state(Component::State_Level_2);
  } else {
    sidebar->set_state(Component::State_Level_1);
  }

  sidebar->update_window_width();
}

void Widget::SideBar::on_persona_click(const PersonaType type, bool active,
                                       void *data) {

  Component *sidebar = (Component *)data;

  if (active) {
    sidebar->content.personas_editor.set_persona(type);
    sidebar->set_state(Component::State_Level_2);
  } else {
    sidebar->set_state(Component::State_Level_1);
  }

  sidebar->update_window_width();
}
