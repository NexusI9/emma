#ifndef _WIDGET_SIDEBAR_CONTENT_PERSONAS_H_
#define _WIDGET_SIDEBAR_CONTENT_PERSONAS_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/geometry/core.h"
#include "runtime/manager/theme.h"
#include "runtime/node/persona.h"
#include "runtime/widgets/sidebar/content/content.hpp"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Personas {

class Component : public Content::Component {

public:
  Component(Gui *);

  void layout() override;
  void draw() override;

private:
  const ImVec2 padding =
      gui_scale_im_vec2(gui, ImVec2(emma_size(ThemeEmmaSize_Space_Medium),
                                    emma_size(ThemeEmmaSize_Space_Medium)));

  const ImVec2 size = gui_scale_im_vec2(gui, ImVec2(311, 66));
  const float radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  const float thickness =
      gui_scale(gui, emma_size(ThemeEmmaSize_Width_Border_Base));

  const float base_y =
      gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_4));

  const float gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Large));

  struct persona {
    ::Component::Sprite avatar;
    const char *label;
    RectCoordinate frame_coo;
    RectCoordinate avatar_coo;
    ImVec2 label_coo;
  } personas[PersonaType_COUNT];

  void draw_card(struct persona *, bool);

  int8_t active_persona = -1;
};

} // namespace Personas
} // namespace Content
} // namespace SideBar
} // namespace Widget

#endif
