#ifndef _WIDGET_SIDEBAR_CONTENT_MODULE_EDITOR_H_
#define _WIDGET_SIDEBAR_CONTENT_MODULE_EDITOR_H_

#include "runtime/manager/module.h"
#include "runtime/solutions/formula.h"
#include "runtime/solutions/globals.h"
#include "runtime/solutions/module/compounds/experience.h"
#include "runtime/widgets/input/input.hpp"
#include "runtime/widgets/sidebar/content/content.hpp"
#include "runtime/widgets/sidebar/content/editor.hpp"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Modules {

namespace Editor {

class Component : public Content::Editor::Component {

public:
  Component(Gui *gui)
      : Content::Editor::Component(gui, "Module Profile", sections,
                                   SECTIONS_COUNT) {}

  void set_module(const ModuleType module) {

    ::Solution *solution = g_module_solutions[module];

    const solution_formula *formulas =
        solution_get_formulas(SolutionType_Module);

    update_motivation(solution, formulas);
    // TODO: find a way to streamline and unify the input update process
    update_inputs_values(module);
    update_inputs_layout();

    set_name(MODULE_INTRO[module].label);
    set_description(MODULE_INTRO[module].description);
  }

private:

  // Update inputs data according to selected persona
  void update_inputs_values(const ModuleType);

  typedef enum {
    SectionType_Experience,
    SectionType_Semantic,
    SectionType_Social,
    SectionType_Actions,
  } SectionType;

  static constexpr uint8_t SECTIONS_COUNT = 4;
  Content::Editor::Section sections[SECTIONS_COUNT] = {

      [SectionType_Experience] =
          {
              .section_header = {gui, "Experience"},
              .input_renderer = {gui,
                                 &sections[SectionType_Experience].input_list},
              .input_list =
                  {
                      .count = 2,
                      .entries =
                          {
                              {
                                  .type = Input::Type_Segment,
                                  .segment =
                                      {
                                          "Impact",
                                          COMPOUND_MODULE_IMPACT_LABELS,
                                          CompoundModuleExperienceImpact_COUNT,
                                      },
                              },
                              {
                                  .type = Input::Type_Segment,
                                  .segment =
                                      {
                                          "Learnability",
                                          COMPOUND_MODULE_LEARNABILITY_LABELS,
                                          CompoundModuleExperienceLearnability_COUNT,
                                      },
                              },

                          },
                  },
          },
      [SectionType_Semantic] =
          {
              .section_header = {gui, "Semantic"},
              .input_renderer = {gui,
                                 &sections[SectionType_Semantic].input_list},
              .input_list =
                  {
                      .count = 2,
                      .entries =
                          {
                              {
                                  .type = Input::Type_Slider,
                                  .slider = {"Empowerment"},
                              },
                              {
                                  .type = Input::Type_Slider,
                                  .slider = {"Epic Meaning"},
                              },
                          },
                  },
          },
      [SectionType_Social] =
          {
              .section_header = {gui, "Social"},
              .input_renderer = {gui, &sections[SectionType_Social].input_list},
              .input_list =
                  {
                      .count = 4,
                      .entries =
                          {
                              {
                                  .type = Input::Type_Toggle,
                                  .toggle = {"Collaborative"},
                              },
                              {
                                  .type = Input::Type_Toggle,
                                  .toggle = {"Competitive"},
                              },
                              {
                                  .type = Input::Type_Toggle,
                                  .toggle = {"Published"},
                              },
                              {
                                  .type = Input::Type_Toggle,
                                  .toggle = {"Shareable"},
                              },
                          },
                  },
          },
      [SectionType_Actions] =
          {
              .section_header = {gui, "Actions"},
              .input_renderer = {gui,
                                 &sections[SectionType_Actions].input_list},
              // sine actions are different for each module we generate them
              // dynamically
              .input_list = {},
          },

  };
};

} // namespace Editor

} // namespace Modules

} // namespace Content

} // namespace SideBar

} // namespace Widget

#endif
