#ifndef _WIDGET_SIDEBAR_BUTTON_H_
#define _WIDGET_SIDEBAR_BUTTON_H_

#include "nkengine/include/texture.h"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/utils.hpp"

namespace Widget {

namespace SideBar {

namespace Button {

class Component : public ::Widget::Widget {

public:
  Component(Gui *gui, const TextureAtlasRegion *icon)
      : Widget(gui), icon(icon) {}

  bool update();
  void render();

  void set_position(ImVec2 pos) {
    position = pos;
    endpoint = ImVec2(position.x + width, position.y + height);
  }

  // clang-format off
  const float width = gui_scale(gui, 52);
  const float height = gui_scale(gui, emma_size(ThemeEmmaSize_Height_Input_Small));
  const float bd_radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  const float bd_thickness = gui_scale(gui, 1.0f);
  
  const ImColor bg_color = emma_im_color(ThemeEmmaColor_Surface_Lower);
  const ImColor bd_color = emma_im_color(ThemeEmmaColor_Border_Subtle_On_Dark);
  const ImColor bg_active_color = emma_im_color(ThemeEmmaColor_Background_Brand_Strong);
  const ImColor bd_active_color = emma_im_color(ThemeEmmaColor_Border_Brand_Subtle);
  // clang-format on

private:
  ImVec2 position;
  ImVec2 endpoint;

  bool active = false;
  const TextureAtlasRegion *icon;
};

} // namespace Button
} // namespace SideBar
} // namespace Widget

#endif
