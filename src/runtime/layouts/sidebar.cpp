#include "sidebar.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"

Layout::SideBar::Component::Component(Gui *gui, Canvas *canvas)
    : Core(gui, canvas), tabs{
                             {gui, ui_sprite(UISprite_Persona_Artisan)},
                         } {

  static const ImVec2 buttons_base_position = ImVec2(0, gui_scale(gui, 112));
  static const int gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    tabs[i].set_position(
        ImVec2(buttons_base_position.x,
               (i + 1) * (buttons_base_position.y + gap + tabs[i].height)));
  }
}

void Layout::SideBar::Component::update() {

  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    if (tabs[i].update()) {
      active_tab = (active_tab == i) ? -1 : i;

      for (uint8_t j = 0; j < tab_update_callbacks.count; j++)
        tab_update_callbacks.entries[j].callback(
            active_tab, tab_update_callbacks.entries[j].data);
    }
  }
}

void Layout::SideBar::Component::render() {

  for (uint8_t i = 0; i < TABS_COUNT; i++)
    tabs[i].render();
}
