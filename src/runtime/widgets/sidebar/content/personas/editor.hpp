#ifndef _WIDGET_SIDEBAR_CONTENT_PERSONA_EDITOR_H_
#define _WIDGET_SIDEBAR_CONTENT_PERSONA_EDITOR_H_

#include "resources/theme.emma.h"
#include "runtime/node/motivation.h"
#include "runtime/node/persona.h"
#include "runtime/solutions/formula.h"
#include "runtime/solutions/globals.h"
#include "runtime/solutions/module/formulas.h"
#include "runtime/solutions/persona/compounds/motivation.h"
#include "runtime/solutions/persona/config.h"
#include "runtime/solutions/persona/core.h"
#include "runtime/solutions/solution.h"
#include "runtime/widgets/input/input.hpp"
#include "runtime/widgets/input/renderer.hpp"
#include "runtime/widgets/motivation_overview.hpp"
#include "runtime/widgets/sidebar/content/content.hpp"
#include "runtime/widgets/sidebar/content/editor_section.hpp"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Personas {

namespace Editor {

class Component : public Content::Component {

public:
  Component(Gui *);

  void layout() override;
  void draw() override;

  void set_persona(const PersonaType persona) {

    ::Solution *solution = g_persona_solutions[persona];

    const solution_formula *formulas =
        solution_get_formulas(SolutionType_Persona);

    for (uint8_t i = 0; i < MotivationType_COUNT; i++)
      motivation_set_element(motivation, (MotivationType)i,
                             formulas[i](solution));

    overview.set_motivation(motivation);
    update_inputs_values(persona);
    update_inputs_layout();
  }

private:
  const int ROW_GAP =
      gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large));
  
  MotivationOverview::Component overview;
  ::Motivation *motivation;

  struct SliderItem {
    const char *label;
    float *value;
  };

  static constexpr uint8_t SECTIONS_COUNT = 4;

  struct Sections {
    const char *label;
    Input::List input_list;
    Input::Renderer::Component input_renderer;
  } sections[SECTIONS_COUNT] = {
      {
          .label = "Motivations",
          .input_renderer = {gui, &sections[0].input_list},
          .input_list =
              {
                  .count = 12,
                  .entries =
                      {
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Epic Meaning"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Empowerment"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Social Influence"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Unpredictability"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Avoidance"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Scarcity"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Ownership"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Accomplishment"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Excitement"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Reward"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Social Bounding"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Friction"},
                          },
                      },
              },
      },
      {
          .label = "Social Behavior",
          .input_renderer = {gui, &sections[1].input_list},
          .input_list =
              {
                  .count = 2,
                  .entries =
                      {
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Collaborative"},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Visibility"},
                          },
                      },
              },
      },
      {
          .label = "Agency",
          .input_renderer = {gui, &sections[3].input_list},
          .input_list =
              {
                  .count = 3,
                  .entries =
                      {
                          {
                              .type = Input::Type_ButtonBar,
                              .button_bar =
                                  {"Feedback Span",
                                   COMPOUND_PERSONA_FEEDBACK_SPAN_LABELS,
                                   CompoundPersonaAgencyFeebackSpan_COUNT},
                          },
                          {
                              .type = Input::Type_ButtonBar,
                              .button_bar =
                                  {"Learning Curve",
                                   COMPOUND_PERSONA_LEARNING_CURVE_LABELS,
                                   CompoundPersonaAgencyLearningCurve_COUNT},
                          },
                          {
                              .type = Input::Type_Slider,
                              .slider = {"Risk Tolerance"},
                          },
                      },
              },
      },
      {
          .label = "Density",
          .input_renderer = {gui, &sections[4].input_list},
          .input_list =
              {
                  .count = 1,
                  .entries =
                      {
                          {
                              .type = Input::Type_Slider,
                              .slider = {""},
                          },
                      },
              },
      },
  };

  // set min, max etc. of sliders
  void set_slider_static_attributes(Input::Descriptor *);

  // Update inputs data according to selected persona
  void update_inputs_values(const PersonaType);
  void update_inputs_layout();
};

} // namespace Editor

} // namespace Personas

} // namespace Content

} // namespace SideBar

} // namespace Widget

#endif
