#include "editor.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/widgets/input/input.hpp"



/**
 Link each sections input descriptor values pointer with the right motivation
 value
 */
void Widget::SideBar::Content::Personas::Editor::Component::
    update_inputs_values(const PersonaType persona) {

  // === Link Motivation ===
  {
    float *values[] = {
        &g_persona_solutions[persona]->persona.motivation.epic_meaning,
        &g_persona_solutions[persona]->persona.motivation.empowerment,
        &g_persona_solutions[persona]->persona.motivation.social_influence,
        &g_persona_solutions[persona]->persona.motivation.unpredictability,
        &g_persona_solutions[persona]->persona.motivation.avoidance,
        &g_persona_solutions[persona]->persona.motivation.scarcity,
        &g_persona_solutions[persona]->persona.motivation.ownership,
        &g_persona_solutions[persona]->persona.motivation.accomplishment,
        &g_persona_solutions[persona]->persona.motivation.excitement,
        &g_persona_solutions[persona]->persona.motivation.reward,
        &g_persona_solutions[persona]->persona.motivation.social_bounding,
        &g_persona_solutions[persona]->persona.motivation.friction,
    };

    static const uint8_t count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < count; i++) {
      Input::Descriptor *input = &sections[0].input_list.entries[i];
      input->slider.value = values[i];
      set_slider_static_attributes(input);
    }
  }

  // === Link Social Behavior ===
  {
    float *values[] = {
        &g_persona_solutions[persona]->persona.social.collaborative,
        &g_persona_solutions[persona]->persona.social.visibility,
    };

    static const uint8_t count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < count; i++) {
      Input::Descriptor *input = &sections[1].input_list.entries[i];
      input->slider.value = values[i];
      set_slider_static_attributes(input);
    }
  }

  // === Link Agency ===
  {

    // Slider
    float *slider_values[] = {
        &g_persona_solutions[persona]->persona.agency.risk_tolerance,
    };

    static const uint8_t slider_count =
        sizeof(slider_values) / sizeof(slider_values[0]);

    for (uint8_t i = 0; i < slider_count; i++) {
      Input::Descriptor *input = &sections[2].input_list.entries[i + 2];
      input->slider.value = slider_values[i];
      set_slider_static_attributes(input);
    }

    // Buttons bar
    int *button_values[] = {
        (int *)&g_persona_solutions[persona]->persona.agency.feedback_span,
        (int *)&g_persona_solutions[persona]->persona.agency.learning_curve,
    };

    static const uint8_t button_count =
        sizeof(button_values) / sizeof(button_values[0]);

    for (uint8_t i = 0; i < button_count; i++)
      sections[2].input_list.entries[i].segment.selected =
          button_values[i];
  }

  // === Link Density ===
  {
    float *values[] = {
        &g_persona_solutions[persona]->persona.density.value,
    };

    static const uint8_t count = sizeof(values) / sizeof(values[0]);

    for (uint8_t i = 0; i < count; i++) {
      Input::Descriptor *input = &sections[3].input_list.entries[i];
      input->slider.value = values[i];
      set_slider_static_attributes(input);
    }
  }
}

