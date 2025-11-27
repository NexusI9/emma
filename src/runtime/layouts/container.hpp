#ifndef _LAYOUT_CONTAINER_H_
#define _LAYOUT_CONTAINER_H_

#include "nkengine/include/renderer.h"
#include "runtime/layouts/nav_bar.hpp"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/canvas.hpp"
#include "runtime/widgets/heatmap.hpp"

EXTERN_C_BEGIN

void container_draw_callback(Renderer *, void *);
void container_set_octalysis_state(bool, void *);
void container_set_heatmap_state(bool, void *);
bool container_get_octalysis_state(void *);
bool container_get_heatmap_state(void *);

EXTERN_C_END

namespace Layout {

class Container {

public:
  Container(Gui *, Canvas *, Heatmap *);
  void draw();

  typedef enum {
    DisplayState_Default = 0,
    DisplayState_Octagon = 1 << 0,
    DisplayState_Heatmap = 1 << 1,
  } DisplayState;

  void enable_display_state(const DisplayState state) {
    display_state |= state;
  }

  void disable_display_state(const DisplayState state) {
    display_state &= ~state;
  }

  unsigned int get_display_state() { return display_state; }

  bool display_state_enabled(const DisplayState state) {
    return get_display_state() & state;
  }

  Widget::HeatmapShape heatmap;

private:
  Gui *gui;
  unsigned int display_state;

  Widget::CanvasShape canvas_shape;
  Widget::ToolBarShape tool_bar;
  Layout::NavBar nav_bar;
};

} // namespace Layout

#endif
