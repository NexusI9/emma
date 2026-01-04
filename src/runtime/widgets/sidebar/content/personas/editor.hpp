#ifndef _WIDGET_SIDEBAR_CONTENT_PERSONA_EDITOR_H_
#define _WIDGET_SIDEBAR_CONTENT_PERSONA_EDITOR_H_

#include "runtime/solutions/formula.h"
#include "runtime/solutions/globals.h"
#include "runtime/widgets/sidebar/content/editor.hpp"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Personas {

namespace Editor {

class Component : public Content::Editor::Component {

public:
  Component(Gui *gui)
      : Content::Editor::Component(gui, "Persona Profile", sections,
                                   SECTIONS_COUNT) {}

  void set_persona(const PersonaType persona) {

    ::Solution *solution = g_persona_solutions[persona];

    const solution_formula *formulas =
        solution_get_formulas(SolutionType_Persona);

    update_motivation(solution, formulas);

    // TODO: find a way to streamline and unify the input update process
    update_inputs_values(persona);
    update_inputs_layout();

    set_name(PERSONA_INTRO[persona].label);
    set_description(PERSONA_INTRO[persona].description);
  }

private:
  // Update inputs data according to selected persona
  void update_inputs_values(const PersonaType);

  static constexpr uint8_t SECTIONS_COUNT = 4;
  Content::Editor::Section
      sections[SECTIONS_COUNT] =
          {

              {
                  .section_header = {gui, "Motivations"},
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
                  .section_header = {gui, "Social Behavior"},
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
                  .section_header = {gui, "Agency"},
                  .input_renderer = {gui, &sections[2].input_list},
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
                  .section_header = {gui, "Density"},
                  .input_renderer = {gui, &sections[3].input_list},
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

}; // namespace Editor

} // namespace Editor

} // namespace Personas

} // namespace Content

} // namespace SideBar

} // namespace Widget

#endif
