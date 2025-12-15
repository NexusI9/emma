#ifndef _WIDGET_SIDEBAR_H_
#define _WIDGET_SIDEBAR_H_

#include "runtime/layouts/core.hpp"
#include "runtime/node/canvas.h"
#include "runtime/widgets/sidebar/content/content.hpp"
#include "runtime/widgets/sidebar/content/modules.hpp"
#include "runtime/widgets/sidebar/panel.hpp"
#include "runtime/widgets/sidebar/tab_button.hpp"

namespace Widget {

namespace SideBar {

typedef void (*on_tab_update)(const uint8_t, void *);

typedef struct {
  on_tab_update callback;
  void *data;
} TabUpdateCallbackEntry;

class Component : public Widget, public Layout::Window {

public:
  Component(const char *, Gui *, ::Canvas *);
  void draw();

  StaticListStatus add_tab_update_callback(on_tab_update callback, void *data) {

    TabUpdateCallbackEntry entry = {callback, data};

    return stli_insert(tab_update_callbacks.entries, CALLBACK_CAPACITY,
                       &tab_update_callbacks.count,
                       sizeof(TabUpdateCallbackEntry), &entry,
                       "Tab Update Callback List");
  }

  // Used to set per-module respective API calls
  // NOTE: don't forget to sync with the 'contents' array bellow
  struct {
    Content::Modules::Component modules;
  } content;

private:
  static constexpr uint8_t CALLBACK_CAPACITY = 8;
  static constexpr uint8_t TABS_COUNT = 1;
  int8_t active_tab = -1;

  ImVec2 position, size, default_size;

  Panel::Component panel;
  TabButton::Component tabs[TABS_COUNT];

  // Used for draw/update auto call depending on active tab
  Content::Component *contents[TABS_COUNT] = {
      &content.modules,
  };

  STATIC_LIST(TabUpdateCallbackEntry, CALLBACK_CAPACITY) tab_update_callbacks;
};

} // namespace SideBar

} // namespace Widget

#endif
