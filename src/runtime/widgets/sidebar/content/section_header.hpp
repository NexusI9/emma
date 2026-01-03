#ifndef _WIDGET_SIDEBAR_SECTION_HEADER_H_
#define _WIDGET_SIDEBAR_SECTION_HEADER_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/vendor/imgui/imgui_internal.h"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/input.hpp"

namespace Widget {

namespace SideBar {

namespace SectionHeader {

class Component : public Widget {

public:
  Component(Gui *gui, const char *label)
      : Widget(gui), label(label),
        info_icon(texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Icon_Info)) {
    layout();
  }

  void layout() {

    info_icon.set_position_x(SIZE.x - ICON_SIZE, GuiSpriteAnchor_TopLeft);

    underline_p0.y = gui_scale(gui, 16) + GAP;

    underline_p1.x = UNDERLINE_SIZE.x;
    underline_p1.y = underline_p0.y + UNDERLINE_SIZE.y;
  };

  void draw() {

    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 origin = ImGui::GetCursorScreenPos();

    ImGui::ItemSize(SIZE);

    ImGui::SetWindowFontScale(1.2);
    dl->AddText(origin, LABEL_COLOR, label);
    ImGui::SetWindowFontScale(1);

    info_icon.draw_at(origin);

    dl->AddRectFilled(im_vec2_add(underline_p0, origin),
                      im_vec2_add(underline_p1, origin), UNDERLINE_COLOR);
  };

private:
  const char *label;
  ::Component::Sprite info_icon;

  const ImVec2 SIZE = ImVec2(gui_scale(gui, 368), gui_scale(gui, 30));
  const int GAP = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Large));
  const int ICON_SIZE = gui_scale(gui, emma_size(ThemeEmmaSize_Icon_Small));
  const ImVec2 UNDERLINE_SIZE = ImVec2(
      SIZE.x, gui_scale(gui, emma_size(ThemeEmmaSize_Width_Border_Base)));

  const ImColor LABEL_COLOR = im_color(emma_color(ThemeEmmaColor_Text_On_Dark));
  const ImColor UNDERLINE_COLOR =
      im_color(emma_color(ThemeEmmaColor_Border_Subtle_On_Dark));

  ImVec2 icon_p;
  ImVec2 underline_p0, underline_p1;
};

} // namespace SectionHeader
} // namespace SideBar
} // namespace Widget

#endif
