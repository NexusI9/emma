#include "editor.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/solutions/globals.h"
#include "runtime/widgets/input/input.hpp"

/**
 Link each sections input descriptor values pointer with the right motivation
 value
 */
void Widget::SideBar::Content::Personas::Editor::Component::
    update_inputs_values(const PersonaType persona) {

  ::Solution *solution = global_persona_solution(persona);

  // === Link Motivation ===
  {
    float *values[] = {
        &solution->persona.motivation.epic_meaning,
        &solution->persona.motivation.empowerment,
        &solution->persona.motivation.social_influence,
        &solution->persona.motivation.unpredictability,
        &solution->persona.motivation.avoidance,
        &solution->persona.motivation.scarcity,
        &solution->persona.motivation.ownership,
        &solution->persona.motivation.accomplishment,
        &solution->persona.motivation.excitement,
        &solution->persona.motivation.reward,
        &solution->persona.motivation.social_bounding,
        &solution->persona.motivation.friction,
    };

    static const uint8_t count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < count; i++) {
      Input::Params *input = &sections[0].input_list.entries[i];
      input->slider.value = values[i];
      set_slider_static_attributes(input);
    }
  }

  // === Link Social Behavior ===
  {
    float *values[] = {
        &solution->persona.social.collaborative,
        &solution->persona.social.visibility,
    };

    static const uint8_t count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < count; i++) {
      Input::Params *input = &sections[1].input_list.entries[i];
      input->slider.value = values[i];
      set_slider_static_attributes(input);
    }
  }

  // === Link Agency ===
  {

    // Slider
    float *slider_values[] = {
        &solution->persona.agency.risk_tolerance,
    };

    static const uint8_t slider_count =
        sizeof(slider_values) / sizeof(slider_values[0]);

    for (uint8_t i = 0; i < slider_count; i++) {
      Input::Params *input = &sections[2].input_list.entries[i + 2];
      input->slider.value = slider_values[i];
      set_slider_static_attributes(input);
    }

    // Buttons bar
    int *button_values[] = {
        (int *)&solution->persona.agency.feedback_span,
        (int *)&solution->persona.agency.learning_curve,
    };

    static const uint8_t button_count =
        sizeof(button_values) / sizeof(button_values[0]);

    for (uint8_t i = 0; i < button_count; i++)
      sections[2].input_list.entries[i].segment.selected = button_values[i];
  }

  // === Link Density ===
  {
    float *values[] = {
        &solution->persona.density.value,
    };

    static const uint8_t count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < count; i++) {
      Input::Params *input = &sections[3].input_list.entries[i];
      input->slider.value = values[i];
      set_slider_static_attributes(input);
    }
  }
}
