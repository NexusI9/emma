#include "editor.hpp"

/**
 Link each sections input descriptor values pointer with the right motivation
 value
 */
void Widget::SideBar::Content::Modules::Editor::Component::update_inputs_values(
    const ModuleType module) {

  // === Link Experience ===
  {
    int *values[] = {
        (int *)&g_module_solutions[module]->module.experience.impact,
        (int *)&g_module_solutions[module]->module.experience.learnability,
    };

    static const uint8_t count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < count; i++) {
      Input::Descriptor *input = &sections[0].input_list.entries[i];
      input->button_bar.selected = values[i];
    }
  }

  // === Link Semantic ===
  {
    float *values[] = {
        &g_module_solutions[module]->module.semantic.empowerment,
        &g_module_solutions[module]->module.semantic.epic_meaning,
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
        &g_module_solutions[module]->module.social.collaborative,
        &g_module_solutions[module]->module.social.competitive,
        &g_module_solutions[module]->module.social.published,
        &g_module_solutions[module]->module.social.shareable,
    };

    static const uint8_t slider_count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < slider_count; i++) {
      Input::Descriptor *input = &sections[2].input_list.entries[i];
      input->toggle.active = values[i];
    }
  }

  // === Link Actions ===
  {
    // TODO
    // float *values[] = {
    //     &g_module_solutions[module]->module.actions.value,
    // };
    //
    //    static const uint8_t count = sizeof(values) / sizeof(values[0]);
    //
    //    for (uint8_t i = 0; i < count; i++) {
    //      Input::Descriptor *input = &sections[3].input_list.entries[i];
    //      input->slider.value = values[i];
    //      set_slider_static_attributes(input);
    //    }
  }
}
