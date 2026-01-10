#ifndef _WIDGET_CHECKBOX_H_
#define _WIDGET_CHECKBOX_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/input.hpp"
#include "webgpu/webgpu.h"

namespace Widget {

namespace Checkbox {

class Component : public Widget {

public:
  Component() : Widget(nullptr) {}

  void init(Gui *gui, Input::CheckboxParams *params) {

    this->gui = gui;
    this->params = params;

    sizes.boundbox = ImVec2(gui_scale(gui, 328), gui_scale(gui, 48));
    layout();
  }

  void layout();
  bool draw();

  void set_size(ImVec2 size) {
    this->sizes.boundbox = size;
    layout();
  }

  void set_width(const int value) {
    this->sizes.boundbox.x = value;
    layout();
  }

  void set_height(const int value) {
    this->sizes.boundbox.y = value;
    layout();
  }

private:
  // Attributes
  Input::CheckboxParams *params;

  // Positions
  struct {
    ImVec2 label;
    ImVec2 outer_box;
    ImVec2 inner_box_start;
    ImVec2 inner_box_end;
  } positions;

  // Sizes
  struct {
    ImVec2 boundbox;
    ImVec2 outer_box;
    ImVec2 inner_box;
    int outer_radius;
    int inner_radius;
    int gap;
  } sizes;

  // Colors
  struct {
    const ImColor label = im_color(emma_color(ThemeEmmaColor_Text_On_Dark));
    const ImColor stroke =
        im_color(emma_color(ThemeEmmaColor_Border_Brand_Base));
    const ImColor background =
        im_color(emma_color(ThemeEmmaColor_Background_Brand_Base));
  } colors;
};

} // namespace Checkbox
} // namespace Widget

#endif
