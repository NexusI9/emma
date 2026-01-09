#ifndef _WIDGET_COMBOBOX_H_
#define _WIDGET_COMBOBOX_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/input.hpp"
#include "webgpu/webgpu.h"

namespace Widget {

namespace Combobox {

class Component : public Widget {

public:
  Component()
      : Widget(nullptr),
        components{
            .chevron_up =
                {
                    texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                    ui_sprite(UISprite_Icon_Chevron_Up_Medium_Black),
                },
            .chevron_down =
                {
                    texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                    ui_sprite(UISprite_Icon_Chevron_Down_Medium_Black),
                },
        } {}

  void init(Gui *gui, Input::ComboboxParams *params) {

    this->gui = gui;
    this->params = params;

    name_compose(id, "##%s_combobox", params->label);

    sizes.boundbox = ImVec2(gui_scale(gui, 328), gui_scale(gui, 48));
    layout();
  }

  void layout();
  void draw();

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
  Input::ComboboxParams *params;
  name_t id;

  typedef enum {
    State_Closed,
    State_Open,
  } State;

  State state;

  // Positions
  struct {
    ImVec2 start;
    ImVec2 end;
    ImVec2 label;
    ImVec2 value;
  } positions;

  // Sizes
  struct {
    ImVec2 boundbox;
    int radius;
    int gap;
    int padding_left;
    int padding_right;
    int padding_bottom;
    int padding_top;
    int value_width;
    int popup_width;
  } sizes;

  // Colors
  struct {
    const ImColor label = im_color(emma_color(ThemeEmmaColor_Text_On_Dark));
    const ImColor value = im_color(emma_color(ThemeEmmaColor_Text_On_Light));
    const ImColor stroke =
        im_color(emma_color(ThemeEmmaColor_Border_Brand_Subtle));
    const ImColor background =
        im_color(emma_color(ThemeEmmaColor_Surface_Highest));
  } colors;

  // Components
  struct {
    ::Component::Sprite chevron_up;
    ::Component::Sprite chevron_down;
  } components;

  // Others Resources/ Caches
  const WGPUTextureView view =
      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI);
};

} // namespace Combobox
} // namespace Widget

#endif
