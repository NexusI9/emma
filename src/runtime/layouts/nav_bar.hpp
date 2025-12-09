#ifndef _LAYOUT_NAV_BAR_H_
#define _LAYOUT_NAV_BAR_H_

#include "nkengine/include/gui.hpp"
#include "runtime/node/canvas.h"
#include <imgui/imgui.h>

namespace Layout {

typedef void (*navbar_switch_set_state)(bool, void *);
typedef bool (*navbar_switch_get_state)(void *);

typedef struct {
  const bool init_state;
  navbar_switch_set_state setter;
  navbar_switch_get_state getter;
  void *user_data;
} NavBarSwitchConfig;

class NavBar {

public:
  NavBar(Gui *, Canvas *, NavBarSwitchConfig, NavBarSwitchConfig);
  void update();
  void render();

  // TODO make it a global theme
  static constexpr ImVec2 margin = ImVec2(50.0, 20.0);
  static constexpr uint8_t height = 42;
  static constexpr ImColor bg_color = ImColor(52, 52, 52, 255);
  static constexpr ImColor bd_color = ImColor(74, 74, 74, 255);
  static constexpr uint8_t bd_radius = 3;
  static constexpr ImVec2 padding = ImVec2(12, 6);

private:
  Gui *gui;
  Canvas *canvas;

  NavBarSwitchConfig octalysis_config;
  NavBarSwitchConfig heatmap_config;

  UI::Frame left_panel, right_panel;

  typedef enum {
    DisplaySwitch_Octagon,
    DisplaySwitch_Heatmap,
    DisplaySwitch_COUNT,
  } DisplaySwitch;

  struct {
    UI::Switch component;
    ImVec2 cached_position = ImVec2(0, 0);
  } switches[DisplaySwitch_COUNT];

  void update_switch(const DisplaySwitch, const char *, NavBarSwitchConfig *);

  void draw_switch(const DisplaySwitch, const char *, const float);
};

} // namespace Layout

#endif
