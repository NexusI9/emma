#ifndef _WIDGET_SIDEBAR_CONTENT_PERSONAS_H_
#define _WIDGET_SIDEBAR_CONTENT_PERSONAS_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/geometry/core.h"
#include "runtime/manager/theme.h"
#include "runtime/node/persona.h"
#include "runtime/widgets/sidebar/content/content.hpp"
#include "utils/callback.h"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Personas {

typedef void (*on_persona_click)(const PersonaType, bool, void *);

typedef CALLBACK_ENTRY(on_persona_click) OnPersonaClickCallback;

class Component : public Content::Component {

public:
  Component(Gui *);

  void layout() override;
  void draw() override;

  void clear_selection() { active_persona = -1; }
  StaticListStatus add_persona_click_callback(on_persona_click cb, void *data) {
    OnPersonaClickCallback entry = {cb, data};
    return stli_insert(persona_click_callbacks.entries, CALLBACK_CAPACITY,
                       &persona_click_callbacks.count,
                       sizeof(OnPersonaClickCallback), &entry,
                       "Persona Click Callback");
  }

private:
  const ImVec2 PADDING =
      gui_scale_im_vec2(gui, ImVec2(emma_size(ThemeEmmaSize_Space_Medium),
                                    emma_size(ThemeEmmaSize_Space_Large)));

  const ImVec2 SIZE = gui_scale_im_vec2(gui, ImVec2(350, 66));
  const float RADIUS = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  const float THICKNESS =
      gui_scale(gui, emma_size(ThemeEmmaSize_Width_Border_Base));

  const float BASE_Y =
      gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_4));

  const float GAP = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Large));
  const float ROW_GAP = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));
  
  struct persona {
    ::Component::Sprite avatar;
    const char *label;
    RectCoordinate frame_coo;
    RectCoordinate avatar_coo;
    ImVec2 label_coo;
  } personas[PersonaType_COUNT];

  bool draw_card(struct persona *, bool);

  static constexpr uint8_t CALLBACK_CAPACITY = 16;
  STATIC_LIST(OnPersonaClickCallback, CALLBACK_CAPACITY)
  persona_click_callbacks;

  int8_t active_persona = -1;
};

} // namespace Personas
} // namespace Content
} // namespace SideBar
} // namespace Widget

#endif
