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
  void listen_update();
  void draw();
  void require_update() { state |= State_RequireUpdate; };
  
  void disable() {
    printf("disable\n");
    size = ImVec2(0, 0);
    flag_enable(State_Hidden, &state);
  }
  void enable() {
    printf("enable\n");
    size = init_size;
    flag_disable(State_Hidden, &state);
  }

private:
  Heatmap::Component heatmaps[Type_COUNT];
  Type active_heatmap = Type_Excitment;
  unsigned int state = State_None;
  UI::Frame list_frame;

  ImVec2 position, size, init_size;

  void draw_list();
};
} // namespace Selector
} // namespace Heatmap

} // namespace Widget

#endif
