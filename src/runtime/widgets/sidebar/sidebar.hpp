#ifndef _WIDGET_SIDEBAR_H_
#define _WIDGET_SIDEBAR_H_

#include "resources/theme.emma.h"
#include "runtime/layouts/core.hpp"
#include "runtime/manager/module.h"
#include "runtime/manager/theme.h"
#include "runtime/node/canvas/core.h"
#include "runtime/node/persona.h"
#include "runtime/widgets/sidebar/content/content.hpp"
#include "runtime/widgets/sidebar/content/modules/editor.hpp"
#include "runtime/widgets/sidebar/content/modules/modules.hpp"
#include "runtime/widgets/sidebar/content/personas/editor.hpp"
#include "runtime/widgets/sidebar/content/personas/personas.hpp"
#include "runtime/widgets/sidebar/panel.hpp"
#include "runtime/widgets/sidebar/tab_button.hpp"

namespace Widget {

namespace SideBar {

typedef void (*on_tab_update)(const uint8_t, void *);

typedef struct {
  on_tab_update callback;
  void *data;
} TabUpdateCallbackEntry;

void on_module_click(const ModuleType, bool, void *);
void on_persona_click(const PersonaType, bool, void *);

class Component : public Widget, public Layout::Window {

public:
  Component(const char *, Gui *, ::Canvas *);
  void layout();
  void draw();
  void close() {
    active_tab = -1;
    set_state(State_Closed);
    update_window_width();
    update_tabs_position();
  }

  StaticListStatus add_tab_update_callback(on_tab_update callback, void *data) {

    TabUpdateCallbackEntry entry = {callback, data};

    return stli_insert(tab_update_callbacks.entries, CALLBACK_CAPACITY,
                       &tab_update_callbacks.count,
                       sizeof(TabUpdateCallbackEntry), &entry,
                       "Tab Update Callback List");
  }

  enum State {
    State_Closed,
    State_Level_1,
    State_Level_2,
  } state;

  void set_state(const State state) { this->state = state; }
  void update_window_width() {

    switch (state) {

    case State_Closed:
      size.x = default_size.x;
      break;

      // expand width on active
    case State_Level_1:
      size.x = panel[0].p1.x;
      panel[0].enable_border_radius();
      break;

    case State_Level_2:
      size.x = panel[1].p1.x;
      panel[0].disable_border_radius();
      break;
    }
  }
  // Used to set per-module respective API calls
  // NOTE: don't forget to sync with the 'contents' array bellow
  struct {
    Content::Modules::Component modules;
    Content::Personas::Component personas;
    Content::Modules::Editor::Component modules_editor;
    Content::Personas::Editor::Component personas_editor;
  } content;

private:
  const float panel_margin_left = gui_scale(gui, 60);
  const float tab_button_offset =
      -1 * gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large));

  static constexpr uint8_t CALLBACK_CAPACITY = 8;
  static constexpr uint8_t TABS_COUNT = 2;
  int8_t active_tab = -1;

  ImVec2 position, size, default_size, bar_p1, shadow_p0, shadow_p1;

  Panel::Component panel[2];
  TabButton::Component tabs[TABS_COUNT];

  // Used for draw/update auto call depending on active tab
  Content::Component *contents[TABS_COUNT] = {
      &content.modules,
      &content.personas,
  };

  Content::Component *sub_contents[TABS_COUNT] = {
      &content.modules_editor,
      &content.personas_editor,
  };

  STATIC_LIST(TabUpdateCallbackEntry, CALLBACK_CAPACITY) tab_update_callbacks;
  void draw_transparent_bar();
  void draw_shadow_bar();

  void update_tabs_position();
};

} // namespace SideBar

} // namespace Widget

#endif
