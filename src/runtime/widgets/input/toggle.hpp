#ifndef _WIDGET_TOGGLE_H_
#define _WIDGET_TOGGLE_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/input.hpp"
#include "webgpu/webgpu.h"

namespace Widget {

namespace Toggle {

class Component : public Widget {

public:
  Component()
      : Widget(nullptr),
        components{
            .button_gradient = {texture_atlas_layer_view(&g_atlas,
                                                         TextureAtlasLayer_UI),
                                ui_sprite(UISprite_Button_Gradient)},
        } {}

  void init(Gui *gui, Input::ToggleParams *params) {

    this->gui = gui;
    this->params = params;

    sizes.boundbox =
        ImVec2(gui_scale(gui, 368),
               gui_scale(gui, emma_size(ThemeEmmaSize_Height_Input_Large)));
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
  // core Attributes
  Input::ToggleParams *params;

  // positions
  struct {
    ImVec2 label;
    ImVec2 end;
  } positions;

  // sizes
  struct {
    ImVec2 boundbox;
    int radius;
  } sizes;

  // colors
  struct {
    const ImColor label = im_color(emma_color(ThemeEmmaColor_Text_On_Dark));
    const ImColor stroke =
        im_color(emma_color(ThemeEmmaColor_Border_Brand_Subtle));
  } colors;

  // components
  struct {
    ::Component::Sprite button_gradient;
  } components;

  // other
  const WGPUTextureView view =
      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI);
};
} // namespace Toggle
} // namespace Widget

#endif
