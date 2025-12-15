#include "sidebar.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"

Widget::SideBar::Component::Component(const char *label, Gui *gui,
                                      Canvas *canvas)
    : Widget(gui), panel(gui), Window(label),
      tabs{
          {gui, ui_sprite(UISprite_Sidebar_Icon_Module)},
      },
      content{
          {gui},
      } {


  position = ImVec2(0, gui_scale(gui, 92));
  default_size = ImVec2(gui_scale(gui, 54), gui_scale(gui, 776));
  size = default_size;

  static const ImVec2 buttons_base_position = ImVec2(0, gui_scale(gui, 112));
  static const int gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    tabs[i].set_position(
        ImVec2(buttons_base_position.x,
               (i + 1) * (buttons_base_position.y + gap + tabs[i].height)));
  }
}

void Widget::SideBar::Component::draw() {

  UI::DockedWindow().Begin(label, position, size);

  if (active_tab >= 0) {
    panel.begin();
    contents[active_tab]->draw();
    panel.end();
  }

  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    if (tabs[i].update()) {
      active_tab = (active_tab == i) ? -1 : i;
      // expand width on active
      size.x = active_tab > -1 ? panel.size.x : default_size.x;

      for (uint8_t j = 0; j < tab_update_callbacks.count; j++)
        tab_update_callbacks.entries[j].callback(
            active_tab, tab_update_callbacks.entries[j].data);
    }

    tabs[i].render(active_tab == i);
  }

  UI::DockedWindow().End();
}
