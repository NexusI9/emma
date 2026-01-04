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
  typedef enum {
    Format_Float,
    Format_Integer,
  } Format;

  Component()
      : Widget(nullptr),
        knob(texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
             ui_sprite(UISprite_Slider_Knob)) {}

  // since we build slider dynamically in the input list render, we include the
  // dynamic attributes within a init function rather the constructor.
  // This is mostly due to the fact that the input list render use a list or
  // slider and we would need to manually initalize it all 32 sliders at
  // construct, which would not be convenient and elegant.
  void init(Gui *gui, const char *label, float *value, int min = 0, int max = 1,
            int multiplier = 1, Format format = Format_Float) {
    this->gui = gui;
    this->label = label;
    this->value = value;
    this->min = min;
    this->max = max;
    this->multiplier = multiplier;
    this->format = format;
  }

  void layout();
  void draw();

private:
  const char *label;
  float *value;
  name_t str_value;
  void update_value(const float);
  int min, max, multiplier;
  Format format;

  // sizes
  ImVec2 slider_size;
  int dot_size;
  int row_gap;
  int amount_gap;

  // positions
  ImVec2 label_p;
  ImVec2 amount_p;
  ImVec2 slider_p0;
  ImVec2 slider_p1;
  ImVec2 slider_active_p0;
  ImVec2 slider_active_p1;

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
