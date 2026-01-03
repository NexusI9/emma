#ifndef _EMMA_WIDGET_HEAT_MAP_SELECT_H_
#define _EMMA_WIDGET_HEAT_MAP_SELECT_H_

#include "runtime/layouts/core.hpp"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/heatmap.hpp"

namespace Widget {

namespace Heatmap {

namespace Selector {

typedef enum {
  Type_Excitment,
  Type_Reward,
  Type_Social,
  Type_Challenge,
  Type_COUNT,
} Type;

typedef enum {
  State_None = 0,
  State_WheelMove = 1 << 0,
  State_RequireUpdate = 1 << 1,
  State_Hidden = 1 << 2,
} State;

class Component : public Widget, public Layout::Window {

public:
  Component(const char *, Gui *, ::Heatmap *[Type_COUNT]);

  void layout();
  void draw();

  void listen_update();
  void require_update() { state |= State_RequireUpdate; };

  /*
    Since we use a fixed window indexing using
    ImGuiWindowflags_NoBringToFrontOnFocus, we need to always draw the heatmap
    Window, case dynamically enabling Begin/End, push them at the back of the
    order and always shows it in the background.

    A solution to this is to call the Begin/End at all time, but only the
    content when the state allows it (show/hidden).

    However conditionally calling the content is not enough because since we
    draw the fullscreen window all the time, although we don't call and draw the
    child, the invisible window still blocks the input for the windows bellow
    it.

    As a result we need to set the window size to 0 we disabled so it make the
    input available for below windows.
   */
  void disable() {
    size = ImVec2(0, 0);
    flag_enable(State_Hidden, &state);
  }
  void enable() {
    size = init_size;
    flag_disable(State_Hidden, &state);

    for (uint8_t i = 0; i < Type_COUNT; i++)
      heatmaps[i].update_relative_motivations();
  }

private:
  Heatmap::Component heatmaps[Type_COUNT];
  Type active_heatmap = Type_Excitment;
  unsigned int state = State_None;
  UI::Frame list_frame;

  ImVec2 list_position, list_window_position, size, init_size;
  name_t list_name;

  void draw_list();
};
} // namespace Selector
} // namespace Heatmap

} // namespace Widget

#endif
