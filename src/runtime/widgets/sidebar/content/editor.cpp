#include "editor.hpp"
#include "runtime/node/motivation.h"


void Widget::SideBar::Content::Editor::Component::layout() {}

void Widget::SideBar::Content::Editor::Component::draw() {

  draw_header(header);

  {
    ImGui::SetWindowFontScale(1.2);
    ImGui::Text("%s", name);
    ImGui::SetWindowFontScale(1);
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() +
                           ImGui::GetContentRegionAvail().x);
    ImGui::TextWrapped("%s", description);
    ImGui::PopTextWrapPos();
  }

  ImGui::Dummy(ImVec2(0, ROW_GAP));

  overview.draw();

  ImGui::Dummy(ImVec2(0, ROW_GAP));

  for (uint8_t i = 0; i < sections_count; i++) {

    // draw header
    sections[i].section_header.draw();
    ImGui::Dummy(ImVec2(0, ROW_GAP));

    // draw input
    sections[i].input_renderer.draw();
    ImGui::Dummy(ImVec2(0, ROW_GAP));
  }
}

void Widget::SideBar::Content::Editor::Component::set_slider_static_attributes(
    Input::Params *input) {

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
   Once the descriptors have the right values, we init and layout the input
   renderer entries according to the previously set input. Note that the order
   assign/link value ==> init/layout is important.
   The overall flow is:

   Select Persona => Fetch Info => Update Input DATA => Update Input Widget
 */
void Widget::SideBar::Content::Editor::Component::update_inputs_layout() {

  for (uint8_t i = 0; i < sections_count; i++) {
    sections[i].input_renderer.init();
    sections[i].input_renderer.layout();
  }
}

void Widget::SideBar::Content::Editor::Component::update_motivation(
    const ::Solution *solution, const solution_formula *formulas) {

  for (uint8_t i = 0; i < MotivationType_COUNT; i++)
    motivation_set_element(motivation, (MotivationType)i,
                           formulas[i](solution));

  // DEBUG
  motivation_print(motivation);

  overview.set_motivation(motivation);
}
