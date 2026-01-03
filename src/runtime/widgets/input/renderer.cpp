#include "renderer.hpp"
#include "runtime/widgets/input/input.hpp"
#include "runtime/widgets/input/slider.hpp"
#include <cstring>

/**
   Cache and initialise each input in the lists according to the list data.
 */
void Widget::Input::Renderer::Component::init() {

  reset_cursors();

  for (uint8_t i = 0; i < list->count; i++) {

    Input::Descriptor *input = &list->entries[i];

    switch (input->type) {

    case Type_Slider:
      sliders[cursors[Type_Slider]].init(gui, input->slider.label,
                                         input->slider.value, input->slider.min,
                                         input->slider.max);
      break;

    case Type_Toggle:
      break;

    case Type_ButtonBar:
      break;

    case Type_Amount:
      break;

    case Type_Combobox:
      break;

    case Type_Checkbox:
      break;

    case Type_Action:
      break;

    default:
      break;
    }

    cursors[list->entries[i].type]++;
  }
}

void Widget::Input::Renderer::Component::layout() {

  reset_cursors();

  for (uint8_t i = 0; i < list->count; i++) {

    switch (list->entries[i].type) {

    case Type_Slider:
      sliders[cursors[Type_Slider]].layout();
      break;

    case Type_Toggle:
      break;

    case Type_ButtonBar:
      break;

    case Type_Amount:
      break;

    case Type_Combobox:
      break;

    case Type_Checkbox:
      break;

    case Type_Action:
      break;

    default:
      break;
    }

    cursors[list->entries[i].type]++;
  }
}

void Widget::Input::Renderer::Component::draw() {

  reset_cursors();

  for (uint8_t i = 0; i < list->count; i++) {

    switch (list->entries[i].type) {

    case Type_Slider:
      sliders[cursors[Type_Slider]].draw();
      break;

    case Type_Toggle:

      break;

    case Type_ButtonBar:
      break;

    case Type_Amount:
      break;

    case Type_Combobox:
      break;

    case Type_Checkbox:
      break;

    case Type_Action:
      break;

    default:
      break;
    }

    cursors[list->entries[i].type]++;
    ImGui::Dummy(ImVec2(0, INPUT_GAP));
  }
}
