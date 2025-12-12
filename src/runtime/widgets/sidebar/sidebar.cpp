#include "sidebar.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"

Widget::SideBar::Component::Component(Gui *gui, Canvas *canvas)
    : Widget(gui), panel(gui),
      tabs{
          {gui, ui_sprite(UISprite_Sidebar_Icon_Module)},
      },
      content{
          {gui},
      } {

  static const ImVec2 buttons_base_position = ImVec2(0, gui_scale(gui, 112));
  static const int gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    tabs[i].set_position(
        ImVec2(buttons_base_position.x,
               (i + 1) * (buttons_base_position.y + gap + tabs[i].height)));
  }
}

void Widget::SideBar::Component::update() {

  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    if (tabs[i].update()) {
      active_tab = (active_tab == i) ? -1 : i;

      for (uint8_t j = 0; j < tab_update_callbacks.count; j++)
        tab_update_callbacks.entries[j].callback(
            active_tab, tab_update_callbacks.entries[j].data);
    }
  }

  if (active_tab >= 0) {
    // set the cursor to the panel position so the update function can access it
    // with GetCursorPos() and have relative coordinate to panel content
    ImGui::SetCursorPos(panel.content_inner_position);
    contents[active_tab]->update();
  }
}

void Widget::SideBar::Component::render() {

  if (active_tab >= 0) {
    panel.begin();
    contents[active_tab]->render();
    panel.end();
  }

  for (uint8_t i = 0; i < TABS_COUNT; i++) {
    tabs[i].render(active_tab == i);
  }
}
