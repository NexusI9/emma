#ifndef _LAYOUT_CONTAINER_H_
#define _LAYOUT_CONTAINER_H_

#include "nkengine/include/renderer.h"
#include "runtime/layouts/nav_bar.hpp"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/canvas/canvas.hpp"
#include "runtime/widgets/heatmap.hpp"

EXTERN_C_BEGIN

void container_draw_callback(Renderer *, void *);
void container_set_octalysis_state(bool, void *);
void container_set_heatmap_state(bool, void *);
bool container_get_octalysis_state(void *);
bool container_get_heatmap_state(void *);
void on_toolbar_update(const uint8_t, void *);

EXTERN_C_END

namespace Layout {

class Container {

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

  Container(Gui *, Canvas *, Heatmap[HeatmapType_COUNT]);
  void draw();

  void enable_display_state(const DisplayState state) {
    flag_enable(state, &display_state);

    switch (state) {

    case DisplayState_Heatmap:
      canvas_shape.enable_state(Widget::CanvasShape::State_FreezeSelection);
      break;

    case DisplayState_Octagon:
      canvas_shape.enable_state(Widget::CanvasShape::State_ShowOctagon);
      break;

    default:
      break;
    }
  }

  void disable_display_state(const DisplayState state) {
    flag_disable(state, &display_state);

    switch (state) {

    case DisplayState_Heatmap:
      canvas_shape.disable_state(Widget::CanvasShape::State_FreezeSelection);
      break;

    case DisplayState_Octagon:
      canvas_shape.disable_state(Widget::CanvasShape::State_ShowOctagon);
      break;

    default:
      break;
    }
  }

  unsigned int get_display_state() { return display_state; }

  bool display_state_enabled(const DisplayState state) {
    return get_display_state() & state;
  }

  void update_canvas_mode(const Widget::CanvasCreate::Mode mode) {
    canvas_shape.update_create_mode(mode);
  }

  Widget::HeatmapShape heatmaps[HeatmapType_COUNT];
  HeatmapType active_heatmap = HeatmapType_Excitment;
  unsigned int heatmap_state = HeatmapState_None;

private:
  Gui *gui;
  unsigned int display_state = 0;

  Widget::CanvasShape canvas_shape;
  Widget::ToolBarShape tool_bar;
  Layout::NavBar nav_bar;
  UI::Frame heatmap_list_shape;

  void draw_heatmap_list();
};

} // namespace Layout

#endif
