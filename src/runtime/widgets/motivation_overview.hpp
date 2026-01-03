#ifndef _WIDGET_MOTIVATION_OVERVIEW_H_
#define _WIDGET_MOTIVATION_OVERVIEW_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/node/motivation.h"
#include "runtime/node/octagon.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/octagon.hpp"
#include <imgui/imgui.h>

namespace Widget {

namespace MotivationOverview {

class Component : public ::Widget::Widget {

public:
  Component(Gui *gui, ImVec2 window_position = ImVec2(0, 0));
  void layout();
  void set_origin(ImVec2 origin) { this->origin = origin; }
  void draw();
  void update_gauge(struct Gauge *);
  void set_motivation(const ::Motivation *motivation) {
    this->motivation = motivation;
    octagon_set_outer_offset_from_motivation(octagon.get_node(),
                                             this->motivation);
    update_gauges_mask();
  }

private:
  const ::Motivation *motivation;
  ::Widget::Octagon::Component octagon;

  const ImVec2 SIZE = gui_scale_im_vec2(gui, ImVec2(368, 388));
  const int BORDER_RADIUS =
      gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  const ImColor BACKGROUND =
      im_color(emma_color(ThemeEmmaColor_Surface_Lowest));
  const ImVec2 PADDING = gui_scale_im_vec2(
      gui, ImVec2(emma_size(ThemeEmmaSize_Space_Extra_Large),
                  emma_size(ThemeEmmaSize_Space_Extra_Large_2)));
  const int GAUGE_HEIGHT = gui_scale(gui, 16);
  const int ICON_GAP = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Small));
  const int GAUGE_GAP =
      gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large));
  const int ROW_GAP =
      gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_4));
  const ImVec2 WINDOW_SIZE = gui_scale_im_vec2(gui, ImVec2(368, 388));

  ImVec2 window_position = ImVec2(0, 0);
  ImVec2 origin;

  static constexpr uint8_t GAUGES_COUNT = 4;
  struct Gauge {
    const char *label;
    ::Component::Sprite gradient;
    ::Component::Sprite window;
    ::Component::Sprite leading_icon;
    ::Component::Sprite trailing_icon;
    ImVec2 mask_p0, mask_p1, label_p;
  } gauges[GAUGES_COUNT];

  void update_gauges_mask();
  void draw_gauge(ImDrawList *, struct Gauge *);
}; // namespace MotivationOverview

} // namespace MotivationOverview
} // namespace Widget

#endif
