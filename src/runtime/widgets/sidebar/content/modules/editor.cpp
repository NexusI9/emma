#include "editor.hpp"
#include "runtime/solutions/module/core.h"
#include "runtime/solutions/solution.h"
#include "runtime/widgets/input/input.hpp"
#include "runtime/widgets/sidebar/content/editor.hpp"

/**
   Generate the input list descriptor according to the module type.
 */
void Widget::SideBar::Content::Modules::Editor::Component::
    create_actions_input_descriptor(const ModuleType module) {

  ::Solution *solution = global_module_solution(module);

  Input::List *input_list = &sections[SectionType_Actions].input_list;

  for (size_t i = 0; i < solution->module.actions.count; i++) {

    SolutionModuleAction *action = &solution->module.actions.entries[i];
    Input::Descriptor *desc = &input_list->entries[i];

    desc->type = Input::Type_Action;

    desc->action.label = action->action.label;

    desc->action.active = &action->action.active;
    desc->action.role = &action->action.role;

#define ACTION_REF_MAP(A, B, attr) A->action.attr = &B->attr;

    // link reward properties (by reference)
    ACTION_REF_MAP(desc, action, reward.type);
    ACTION_REF_MAP(desc, action, reward.probability);

    // amount
    ACTION_REF_MAP(desc, action, reward.amount.type);
    ACTION_REF_MAP(desc, action, reward.amount.value);
    ACTION_REF_MAP(desc, action, reward.amount.range.min);
    ACTION_REF_MAP(desc, action, reward.amount.range.max);

    // frequency
    ACTION_REF_MAP(desc, action, reward.frequency.quota);
    ACTION_REF_MAP(desc, action, reward.frequency.interval);
    ACTION_REF_MAP(desc, action, reward.frequency.repeat);
    ACTION_REF_MAP(desc, action, reward.frequency.forever);
    ACTION_REF_MAP(desc, action, reward.frequency.unit);

    // time limit
    ACTION_REF_MAP(desc, action, reward.time_limit.active);
    ACTION_REF_MAP(desc, action, reward.time_limit.amount);
    ACTION_REF_MAP(desc, action, reward.time_limit.unit);
  }

  input_list->count = solution->module.actions.count;

#undef ACTION_REF_MAP
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
      Input::Descriptor *input = &sections[0].input_list.entries[i];
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
      Input::Descriptor *input = &sections[1].input_list.entries[i];
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
      Input::Descriptor *input = &sections[2].input_list.entries[i];
      input->toggle.active = values[i];
    }
  }

  // === Link Actions ===
  create_actions_input_descriptor(module);
}
