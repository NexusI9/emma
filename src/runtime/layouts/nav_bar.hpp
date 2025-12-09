#ifndef _LAYOUT_NAV_BAR_H_
#define _LAYOUT_NAV_BAR_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/node/canvas.h"
#include "runtime/widgets/utils.hpp"
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

  const ImVec2 margin = ImVec2(50.0, 20.0);
  const uint8_t height = 42;
  const float bd_radius = emma_size(ThemeEmmaSize_Radius_Base);
  const ImVec2 padding = ImVec2(12, 6);

private:
  Gui *gui;
  Canvas *canvas;

  NavBarSwitchConfig octalysis_config;
  NavBarSwitchConfig heatmap_config;

  UI::Frame main_panel;

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
