#ifndef _WIDGET_INPUT_RENDERER_H_
#define _WIDGET_INPUT_RENDERER_H_

#include "nkengine/include/list.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/action.hpp"
#include "runtime/widgets/input/input.hpp"
#include "runtime/widgets/input/segment.hpp"
#include "runtime/widgets/input/slider.hpp"
#include "runtime/widgets/input/toggle.hpp"

#include <stdint.h>

namespace Widget {

namespace Input {

namespace Renderer {

class Component : public Widget {

public:
  Component(Gui *gui, Input::List *list) : Widget(gui), list(list){};

  Input::List *list;

  void init();
  void layout();
  void draw();

private:
  const int INPUT_GAP = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  int cursors[Input::Type_COUNT];
  void reset_cursors() { memset(cursors, 0, sizeof(cursors)); }

  void map_action(Input::Descriptor *, Action::Component *);

  // cached inputs
  Slider::Component sliders[INPUT_LIST_CAPACITY];
  Toggle::Component toggles[INPUT_LIST_CAPACITY];
  Segment::Component segments[INPUT_LIST_CAPACITY];
  Action::Component actions[INPUT_LIST_CAPACITY];
};

} // namespace Renderer

} // namespace Input

} // namespace Widget

#endif
