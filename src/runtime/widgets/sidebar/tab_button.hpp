#ifndef _WIDGET_SIDEBAR_BUTTON_H_
#define _WIDGET_SIDEBAR_BUTTON_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/include/texture.h"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/utils.hpp"

namespace Widget {

namespace SideBar {

namespace TabButton {

class Component : public ::Widget::Widget {

public:
  Component(Gui *gui, const TextureAtlasRegion *icon)
      : Widget(gui),
        icon(::Component::Sprite(
            texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI), icon)) {}

  bool update();
  void render(bool);

  void set_position(ImVec2);

  // clang-format off
  const float width = gui_scale(gui, 52);
  const float height = gui_scale(gui, emma_size(ThemeEmmaSize_Height_Input_Small));
  const float icon_size = gui_scale(gui, 26);
  const float icon_padding = gui_scale(gui, 4); 
  const float bd_radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  const float bd_thickness = gui_scale(gui, 1.0f);
  
  const ImColor bg_color = emma_im_color(ThemeEmmaColor_Surface_Lower);
  const ImColor bd_color = emma_im_color(ThemeEmmaColor_Border_Subtle_On_Dark);
  const ImColor bg_active_color = emma_im_color(ThemeEmmaColor_Background_Brand_Strong);
  const ImColor bd_active_color = emma_im_color(ThemeEmmaColor_Border_Brand_Subtle);
  // clang-format on

private:
  ::Component::Sprite icon;
  ImVec2 position, icon_position;
  ImVec2 endpoint;
};

} // namespace Button
} // namespace SideBar
} // namespace Widget

#endif
