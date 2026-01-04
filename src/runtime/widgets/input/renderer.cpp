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
    int *cursor = &cursors[input->type];

    switch (input->type) {

    case Type_Slider:
      sliders[*cursor].init(gui, input->slider.label, input->slider.value,
                            input->slider.min, input->slider.max, 10,
                            Slider::Component::Format_Integer);
      break;

    case Type_Toggle:
      toggles[*cursor].init(gui, input->toggle.label, input->toggle.active);
      break;

    case Type_Segment:
      segments[*cursor].init(gui, input->segment.label, input->segment.items,
                             input->segment.count, input->segment.selected);
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

    (*cursor)++;
  }
}

void Widget::Input::Renderer::Component::layout() {

  reset_cursors();

  for (uint8_t i = 0; i < list->count; i++) {

    Input::Type type = list->entries[i].type;
    int *cursor = &cursors[type];

    switch (type) {

    case Type_Slider:
      sliders[*cursor].layout();
      break;

    case Type_Toggle:
      toggles[*cursor].layout();
      break;

    case Type_Segment:
      segments[*cursor].layout();
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

    (*cursor)++;
  }
}

void Widget::Input::Renderer::Component::draw() {

  reset_cursors();

  for (uint8_t i = 0; i < list->count; i++) {

    Input::Type type = list->entries[i].type;
    int *cursor = &cursors[type];

    switch (type) {

    case Type_Slider:
      sliders[*cursor].draw();
      break;

    case Type_Toggle:
      toggles[*cursor].draw();
      break;

    case Type_Segment:
      segments[*cursor].draw();
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

    (*cursor)++;
    ImGui::Dummy(ImVec2(0, INPUT_GAP));
  }
}
