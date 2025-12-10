#ifndef _LAYOUT_SIDE_BAR_H_
#define _LAYOUT_SIDE_BAR_H_

#include "runtime/layouts/core.hpp"
#include "runtime/node/canvas.h"
#include "runtime/widgets/sidebar_button.hpp"

namespace Layout {

namespace SideBar {

typedef void (*on_tab_update)(const uint8_t, void *);

typedef struct {
  on_tab_update callback;
  void *data;
} TabUpdateCallbackEntry;

class Component : public Core {

public:
  Component(Gui *, Canvas *);
  void update();
  void render();

  StaticListStatus add_tab_update_callback(on_tab_update callback, void *data) {

    TabUpdateCallbackEntry entry = {callback, data};

    return stli_insert(tab_update_callbacks.entries, CALLBACK_CAPACITY,
                       &tab_update_callbacks.count,
                       sizeof(TabUpdateCallbackEntry), &entry,
                       "Tab Update Callback List");
  }

private:
  static constexpr uint8_t CALLBACK_CAPACITY = 8;
  static constexpr uint8_t TABS_COUNT = 1;
  int8_t active_tab = -1;
  Widget::SideBar::Button::Component tabs[TABS_COUNT];

  struct {
    TabUpdateCallbackEntry entries[CALLBACK_CAPACITY];
    size_t count;
  } tab_update_callbacks;
};

} // namespace SideBar

} // namespace Layout

#endif
