#include "editor.hpp"
#include "runtime/solutions/module/core.h"
#include "runtime/solutions/solution.h"
#include "runtime/widgets/input/input.hpp"
#include "runtime/widgets/sidebar/content/editor.hpp"

/**
   On Editor input change we re-read the linked/ active module attribute from
   the global buffer and sync it with the local motivation to display change
   dynamically
 */
void Widget::SideBar::Content::Modules::Editor::update_module_motivation(
    void *user_data) {
  Component *component = (Component *)user_data;
  component->update_motivation_from_active_module();
}

/**
 Link each sections input descriptor values pointer with the right motivation
 value
 */
void Widget::SideBar::Content::Modules::Editor::Component::update_inputs_values(
    const ModuleType module) {

  ::Solution *solution = global_module_solution(module);

  // === Link Experience ===
  {
    int *values[] = {
        (int *)&solution->module.experience.impact,
        (int *)&solution->module.experience.learnability,
    };

    static const uint8_t count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < count; i++) {
      Input::Params *input =
          &sections[SectionType_Experience].input_list.entries[i];
      input->segment.selected = values[i];
    }
  }

  // === Link Semantic ===
  {
    float *values[] = {
        &solution->module.semantic.empowerment,
        &solution->module.semantic.epic_meaning,
    };

    static const uint8_t count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < count; i++) {
      Input::Params *input =
          &sections[SectionType_Semantic].input_list.entries[i];
      input->slider.value = values[i];
      set_slider_static_attributes(input);
    }
  }

  // === Link Social ===
  {

    // Slider
    bool *values[] = {
        &solution->module.social.collaborative,
        &solution->module.social.competitive,
        &solution->module.social.published,
        &solution->module.social.shareable,
    };

    static const uint8_t slider_count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < slider_count; i++) {
      Input::Params *input =
          &sections[SectionType_Social].input_list.entries[i];
      input->toggle.active = values[i];
    }
  }

  // === Link Actions ===
  {
    for (uint8_t i = 0; i < solution->module.actions.count; i++) {
      Input::Params *input =
          &sections[SectionType_Actions].input_list.entries[i];

      input->type = Input::Type_Action;
      input->action.handle = &solution->module.actions.entries[i];
    }

    // update count dynamically for each modules
    sections[SectionType_Actions].input_list.count =
        solution->module.actions.count;
  }
}
