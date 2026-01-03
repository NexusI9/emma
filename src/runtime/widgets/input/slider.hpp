#ifndef _WIDGET_SLIDER_H_
#define _WIDGET_SLIDER_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/include/list.h"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/widgets/core.hpp"
#include "utils/callback.h"
#include <imgui/imgui.h>

namespace Widget {

namespace Slider {

class Component : public Widget {

public:
  Component()
      : Widget(nullptr),
        knob(texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
             ui_sprite(UISprite_Slider_Knob)) {}

  // since we build slider dynamically in the input list render, we include the
  // dynamic attributes within a init function rather the constructor.
  // This is mostly due to the fact that the input list render use a list or
  // slider and we would need to manually initalize it all 32 sliders at
  // construct, which would not be convenient and elegant.
  void init(Gui *gui, const char *label, float *value, int min, int max) {
    this->gui = gui;
    this->label = label;
    this->value = value;
    this->min = min;
    this->max = max;
  }

  void layout();
  void draw();
  void draw_at(ImVec2);

private:
  const char *label;
  float *value;
  void update_value(const float);
  int min, max;

  // sizes
  ImVec2 item_size;
  ImVec2 SLIDER_SIZE;
  int DOT_SIZE;
  int ROW_GAP;

  // positions
  ImVec2 label_p, amout_p, slider_p0, slider_p1, slider_active_p0,
      slider_active_p1;

  ::Component::Sprite knob;

  // colors
  const ImColor ACTIVE_BACKGROUND =
      im_color(emma_color(ThemeEmmaColor_Background_Brand_Base));
  const ImColor DEFAULT_BACKGROUND =
      im_color(emma_color(ThemeEmmaColor_Surface_Highest));
  const ImColor LABEL_COLOR = im_color(emma_color(ThemeEmmaColor_Text_On_Dark));
};

} // namespace Slider
} // namespace Widget

#endif
