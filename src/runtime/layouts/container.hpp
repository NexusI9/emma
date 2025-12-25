#ifndef _LAYOUT_CONTAINER_H_
#define _LAYOUT_CONTAINER_H_

#include "nkengine/include/renderer.h"
#include "runtime/layouts/core.hpp"
#include "runtime/layouts/navbar.hpp"
#include "runtime/manager/module.h"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/canvas/canvas.hpp"
#include "runtime/widgets/heatmap.hpp"
#include "runtime/widgets/heatmap_select.hpp"
#include "runtime/widgets/sidebar/sidebar.hpp"
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

void on_module_drag_begin(const TextureAtlasRegion *, const ModuleType, void *);
void on_module_drag(const TextureAtlasRegion *, const ModuleType, const ImVec2,
                    void *);
void on_module_drag_end(const TextureAtlasRegion *, const ModuleType,
                        const ImVec2, void *);

class Component : public Core {

public:
  typedef enum {
    DisplayState_Default = 0,
    DisplayState_Octagon = 1 << 0,
    DisplayState_Heatmap = 1 << 1,
  } DisplayState;

  Component(Gui *, Canvas *, Heatmap * [Widget::Heatmap::Selector::Type_COUNT]);
  void draw();

  void enable_display_state(const DisplayState state) {
    flag_enable(state, &display_state);

    switch (state) {

    case DisplayState_Heatmap:
      heatmap_selector.enable();
      sidebar.close();
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
      heatmap_selector.disable();
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

  Frame *hovering_canvas_frame(const ImVec2);

  unsigned int get_display_state() { return display_state; }

  bool display_state_enabled(const DisplayState state) {
    return get_display_state() & state;
  }

  Widget::Canvas::Component canvas;
  Widget::ToolBar::Component toolbar;
  Widget::SideBar::Component sidebar;
  Widget::Heatmap::Selector::Component heatmap_selector;
  NavBar::Component navbar;

private:
  unsigned int display_state = 0;
};

} // namespace Container
} // namespace Layout

#endif
