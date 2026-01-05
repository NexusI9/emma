#ifndef _WIDGET_SIDEBAR_EDITOR_COMMON_H_
#define _WIDGET_SIDEBAR_EDITOR_COMMON_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/vendor/imgui/imgui_internal.h"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/solutions/solution.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/input.hpp"
#include "runtime/widgets/input/renderer.hpp"
#include "runtime/widgets/motivation_overview.hpp"
#include "runtime/widgets/sidebar/content/content.hpp"
#include "runtime/widgets/sidebar/content/section_header.hpp"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Editor {

typedef struct {
  SectionHeader::Component section_header;
  Input::List input_list;
  Input::Renderer::Component input_renderer;
} Section;

/**
   Core component being used to initialize render and manage the Persona and
   Module profile allowing to Edit each properties through sliders, combobox and
   and other inputs.
*/
class Component : public Content::Component {

public:
  Component(Gui *gui, const char *header, Section *sections,
            const uint8_t count)
      : Content::Component(gui), header(header), overview(gui),
        sections(sections), sections_count(count) {

    // WARNING make sure to monitor constructor call pattern
    motivation = new_motivation();
  }

  void layout() override;
  void draw() override;

protected:
  // set min, max etc. of sliders

  void set_name(const char *name) { this->name = name; }
  void set_description(const char *desc) { this->description = desc; }

  void set_slider_static_attributes(Input::Descriptor *);
  void update_inputs_layout();
  void update_motivation(const ::Solution *, const solution_formula *);

private:
  const int ROW_GAP =
      gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large));

  MotivationOverview::Component overview;
  ::Motivation *motivation;

  Section *sections;
  const uint8_t sections_count;

  const char *header;
  const char *name;
  const char *description;
};

} // namespace Editor
} // namespace Content

} // namespace SideBar
} // namespace Widget

#endif
