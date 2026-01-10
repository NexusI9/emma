#ifndef _WIDGET_SLIDER_H_
#define _WIDGET_SLIDER_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/include/list.h"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/input.hpp"
#include "utils/callback.h"

#include <imgui/imgui.h>

namespace Widget {

namespace Slider {

class Component : public Widget {

public:
  // TODO: use union for Float/Int value
  typedef enum {
    Format_Float,
    Format_Integer,
  } Format;

  Component()
      : Widget(nullptr), components{
                             .knob = {texture_atlas_layer_view(
                                          &g_atlas, TextureAtlasLayer_UI),
                                      ui_sprite(UISprite_Slider_Knob)},
                         } {}

  // since we build slider dynamically in the input list render, we include the
  // dynamic attributes within a init function rather the constructor.
  // This is mostly due to the fact that the input list render use a list or
  // slider and we would need to manually initalize it all 32 sliders at
  // construct, which would not be convenient and elegant.
  void init(Gui *gui, Input::SliderParams *params,
            Format format = Format_Float) {
    this->gui = gui;
    this->format = format;
    this->params = params;

    name_compose(id, "%s_slider", params->label);
  }

  void layout();
  bool draw();

private:
  // core attribtues
  Input::SliderParams *params;
  name_t str_value, id;
  Format format;
  float t, value_t;

  // sizes
  struct {
    ImVec2 boundbox;
    ImVec2 clickbox;
    ImVec2 slider;
    int dot;
    int row_gap;
    int amount_gap;
  } sizes;

  // positions
  struct {
    ImVec2 label;
    ImVec2 amount;
    ImVec2 slider_start;
    ImVec2 slider_end;
    ImVec2 slider_active_start;
    ImVec2 slider_active_end;
  } positions;

  // components
  struct {
    ::Component::Sprite knob;
  } components;

  // colors
  struct {
    const ImColor bg_active =
        im_color(emma_color(ThemeEmmaColor_Background_Brand_Base));
    const ImColor bg_default =
        im_color(emma_color(ThemeEmmaColor_Surface_Highest));
    const ImColor label = im_color(emma_color(ThemeEmmaColor_Text_On_Dark));
  } colors;

  void update_value(const float);
  void mouse_pos_to_value(const ImVec2 &);
  float update_knob_position_from_value();
};

} // namespace Slider
} // namespace Widget

#endif
