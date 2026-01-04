#ifndef _WIDGET_SEGMENT_H_
#define _WIDGET_SEGMENT_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/widgets/core.hpp"
#include "webgpu/webgpu.h"

namespace Widget {

namespace Segment {

class Component : public Widget {

public:
  Component()
      : Widget(nullptr), button_gradient(texture_atlas_layer_view(
                                             &g_atlas, TextureAtlasLayer_UI),
                                         ui_sprite(UISprite_Button_Gradient)) {}

  void init(Gui *gui, const char *label, const char **items, uint32_t count,
            int *selected) {

    this->gui = gui;
    this->selected = selected;
    this->items = items;
    this->count = (uint32_t)fminf(count, CAPACITY);
    this->label = label;

    size = ImVec2(gui_scale(gui, 368), gui_scale(gui, 54));
    layout();
  }
  void layout();
  void draw();

  void set_size(ImVec2 size) {
    this->size = size;
    layout();
  }

private:
  static constexpr const uint8_t CAPACITY = 8;

  const char *label;
  ImVec2 label_p;

  const char **items;
  uint32_t count;

  int *selected;

  typedef struct {
    const char *label;
    ImVec2 label_p;
    ImVec2 p0;
    ImVec2 p1;
    int radius;
    ImDrawFlags flags;
  } Button;

  Button buttons[CAPACITY];

  ::Component::Sprite button_gradient;
  const WGPUTextureView view =
      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI);

  int RADIUS, GAP;

  const ImColor LABEL_COLOR = im_color(emma_color(ThemeEmmaColor_Text_On_Dark));
  const ImColor STROKE_COLOR =
      im_color(emma_color(ThemeEmmaColor_Border_Subtlest_On_Dark));
};
} // namespace Segment
} // namespace Widget

#endif
