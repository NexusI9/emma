#ifndef _LAYOUT_NAV_BAR_H_
#define _LAYOUT_NAV_BAR_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/layouts/core.hpp"
#include "runtime/manager/theme.h"
#include "runtime/node/canvas/core.h"
#include "runtime/widgets/utils.hpp"
#include <imgui/imgui.h>

namespace Layout {

namespace NavBar {
typedef void (*switch_set_state)(bool, void *);
typedef bool (*switch_get_state)(void *);

typedef struct {
  const bool init_state;
  switch_set_state setter;
  switch_get_state getter;
  void *user_data;
} SwitchConfig;

class Component : public Core, public Window {

public:
  Component(const char *, Gui *, Canvas *, SwitchConfig, SwitchConfig);
  void draw();

  const ImVec2 margin = ImVec2(gui_scale(gui, 50.0), gui_scale(gui, 20.0));
  const uint8_t height = 42;
  const float bd_radius = emma_size(ThemeEmmaSize_Radius_Base);
  const ImVec2 padding = ImVec2(gui_scale(gui, 12), gui_scale(gui, 6));
  const ImVec2 position = ImVec2(0, 0);
  const ImVec2 size =
      ImVec2(gui_scale(gui, context_width()), gui_scale(gui, 88));

  void set_project_name(const char *name) { name_copy(name, project_name); }

private:
  name_t project_name;

  UI::Frame main_panel;

  typedef enum {
    DisplaySwitch_Octagon,
    DisplaySwitch_Heatmap,
    DisplaySwitch_COUNT,
  } DisplaySwitch;

  struct {
    UI::Switch component;
    SwitchConfig config;
  } switches[DisplaySwitch_COUNT];

  void draw_switch(const DisplaySwitch, const char *, const char *,
                   const float);
};
} // namespace NavBar
} // namespace Layout

#endif
