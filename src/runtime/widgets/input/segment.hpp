#ifndef _WIDGET_SEGMENT_H_
#define _WIDGET_SEGMENT_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/input.hpp"
#include "webgpu/webgpu.h"

namespace Widget {

namespace Segment {

typedef struct {
  const char *label;
  ImVec2 label_p;
  ImVec2 p0;
  ImVec2 p1;
  int radius;
  ImDrawFlags flags;
} Button;

class Component : public Widget {

public:
  Component()
      : Widget(nullptr),
        components{
            .button_gradient = {texture_atlas_layer_view(&g_atlas,
                                                         TextureAtlasLayer_UI),
                                ui_sprite(UISprite_Button_Gradient)},
        } {}

  void init(Gui *gui, Input::SegmentParams *params) {

    this->gui = gui;
    this->params = params;
    this->params->count = (uint32_t)fminf(this->params->count, CAPACITY);
    this->sizes.boundbox = ImVec2(gui_scale(gui, 368), gui_scale(gui, 54));

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

  ImVec2 get_size() { return sizes.boundbox; }

private:
  // constants
  static constexpr const uint8_t CAPACITY = 8;

  // core attributes
  Input::SegmentParams *params;

  // positions
  struct {
    ImVec2 label;
  } positions;

  // sizes
  struct {
    ImVec2 boundbox;
    int radius;
    int gap;
  } sizes;

  struct {
    const ImColor label = im_color(emma_color(ThemeEmmaColor_Text_On_Dark));
    const ImColor stroke =
        im_color(emma_color(ThemeEmmaColor_Border_Subtlest_On_Dark));
  } colors;

  // components
  struct {
    Button buttons[CAPACITY];
    ::Component::Sprite button_gradient;
  } components;

  // others
  const WGPUTextureView view =
      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI);
};
} // namespace Segment
} // namespace Widget

#endif
