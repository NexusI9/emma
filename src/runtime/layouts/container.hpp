#ifndef _LAYOUT_CONTAINER_H_
#define _LAYOUT_CONTAINER_H_

#include "nkengine/include/renderer.h"
#include "runtime/layouts/core.hpp"
#include "runtime/layouts/navbar.hpp"
#include "runtime/layouts/sidebar.hpp"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/canvas/canvas.hpp"
#include "runtime/widgets/heatmap.hpp"
#include "runtime/widgets/toolbar.hpp"

namespace Layout {

namespace Container {

void draw_callback(Renderer *, void *);
void set_octalysis_state(bool, void *);
void set_heatmap_state(bool, void *);
bool get_octalysis_state(void *);
bool get_heatmap_state(void *);
void on_toolbar_update(const uint8_t, void *);
void on_sidebar_tab_update(const uint8_t, void *);

class Component : public Core {

public:
  typedef enum {
    DisplayState_Default = 0,
    DisplayState_Octagon = 1 << 0,
    DisplayState_Heatmap = 1 << 1,
  } DisplayState;

  typedef enum {
    HeatmapType_Excitment,
    HeatmapType_Reward,
    HeatmapType_Social,
    HeatmapType_Challenge,
    HeatmapType_COUNT,
  } HeatmapType;

  typedef enum {
    HeatmapState_None = 0,
    HeatmapState_WheelMove = 1 << 0,
    HeatmapState_RequireUpdate = 1 << 1,
  } HeatmapState;

  Component(Gui *, Canvas *, Heatmap[HeatmapType_COUNT]);
  void draw();

  void enable_display_state(const DisplayState state) {
    flag_enable(state, &display_state);

    switch (state) {

    case DisplayState_Heatmap:

      canvas.enable_state(
          Widget::Canvas::Component::State_ShowHeatmap |
          Widget::Canvas::Component::State_FreezeSelection |
          Widget::Canvas::Component::State_FreezeTransform |
          Widget::Canvas::Component::State_FreezeCreationSession);
      break;

    case DisplayState_Octagon:
      canvas.enable_state(
          Widget::Canvas::Component::State_ShowOctagon |
          Widget::Canvas::Component::State_FreezeCreationSession);
      break;

    default:
      break;
    }
  }

  void disable_display_state(const DisplayState state) {
    flag_disable(state, &display_state);

    switch (state) {

    case DisplayState_Heatmap:
      canvas.enable_state(
          Widget::Canvas::Component::State_FreezeCreationSession);
      canvas.disable_state(Widget::Canvas::Component::State_ShowHeatmap |
                           Widget::Canvas::Component::State_FreezeSelection |
                           Widget::Canvas::Component::State_FreezeTransform);
      break;

    case DisplayState_Octagon:
      canvas.enable_state(
          Widget::Canvas::Component::State_FreezeCreationSession);
      canvas.disable_state(Widget::Canvas::Component::State_ShowOctagon);
      break;

    default:
      break;
    }
  }

  void canvas_enable_state(const Widget::Canvas::Component::State state) {
    canvas.enable_state(state);
  }

  void canvas_disable_state(const Widget::Canvas::Component::State state) {
    canvas.disable_state(state);
  }

  unsigned int get_display_state() { return display_state; }

  bool display_state_enabled(const DisplayState state) {
    return get_display_state() & state;
  }

  void update_canvas_mode(const Widget::Canvas::Create::Mode mode) {
    canvas.update_create_mode(mode);
  }

  Widget::Heatmap::Component heatmaps[HeatmapType_COUNT];
  HeatmapType active_heatmap = HeatmapType_Excitment;
  unsigned int heatmap_state = HeatmapState_None;

private:
  unsigned int display_state = 0;

  Widget::Canvas::Component canvas;
  Widget::ToolBar::Component toolbar;
  NavBar::Component navbar;
  SideBar::Component sidebar;
  
  UI::Frame heatmap_list_shape;

  void draw_heatmap_list();
};

} // namespace Container
} // namespace Layout

#endif
