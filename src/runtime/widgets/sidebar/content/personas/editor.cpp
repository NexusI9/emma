#include "editor.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/widgets/input/input.hpp"

Widget::SideBar::Content::Personas::Editor::Component::Component(Gui *gui)
    : Content::Component(gui), overview(gui) {

  // WARNING make sure to monitor constructor call pattern
  motivation = new_motivation();
}

void Widget::SideBar::Content::Personas::Editor::Component::layout() {}

void Widget::SideBar::Content::Personas::Editor::Component::draw() {

  draw_header("Persona Overview");
  overview.draw();

  ImGui::Dummy(ImVec2(0, ROW_GAP));
  draw_header("Edit");

  for (uint8_t i = 0; i < SECTIONS_COUNT; i++) {
    // draw header

    // draw input
    sections[i].input_renderer.draw();
  
  }
}

void Widget::SideBar::Content::Personas::Editor::Component::
    set_slider_static_attributes(Input::Descriptor *input) {

  static constexpr float SLIDER_MIN = 0.0f;
  static constexpr float SLIDER_MAX = 10.0f;
  static constexpr float SLIDER_STEP = 1.0f;
  static constexpr float SLIDER_MULTIPLIER = 10.0f;

  input->slider.min = SLIDER_MIN;
  input->slider.max = SLIDER_MAX;
  input->slider.step = SLIDER_STEP;
  input->slider.multiplier = SLIDER_MULTIPLIER;
}

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
      sections[2].input_list.entries[i].button_bar.selected = button_values[i];
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

/**
   Once the descriptors have the right values, we init and layout the input
   renderer entries according to the previously set input. Note that the order
   assign/link value ==> init/layout is important.
   The overall flow is:

   Select Persona => Fetch Info => Update Input DATA => Update Input Widget
 */
void Widget::SideBar::Content::Personas::Editor::Component::
    update_inputs_layout() {

  for (uint8_t i = 0; i < SECTIONS_COUNT; i++) {
    sections[i].input_renderer.init();
    sections[i].input_renderer.layout();
  }
}
