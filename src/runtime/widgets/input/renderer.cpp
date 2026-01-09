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

    Input::Params *params = &list->entries[i];
    int *cursor = &cursors[params->type];

    switch (params->type) {

    case Type_Slider:
      sliders[*cursor].init(gui, &params->slider,
                            Slider::Component::Format_Integer);
      break;

    case Type_Toggle:
      toggles[*cursor].init(gui, &params->toggle);
      break;

    case Type_Segment:
      segments[*cursor].init(gui, &params->segment);
      break;

    case Type_Amount:
      amounts[*cursor].init(gui, &params->amount);
      break;

    case Type_Combobox:
      comboboxes[*cursor].init(gui, &params->combobox);
      break;

    case Type_Checkbox:
      checkboxes[*cursor].init(gui, &params->checkbox);
      break;

    case Type_Action:
      actions[*cursor].init(gui, &params->action);
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
      amounts[*cursor].layout();
      break;

    case Type_Combobox:
      comboboxes[*cursor].layout();
      break;

    case Type_Checkbox:
      checkboxes[*cursor].layout();
      break;

    case Type_Action:
      actions[*cursor].layout();
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
      amounts[*cursor].draw();
      break;

    case Type_Combobox:
      comboboxes[*cursor].draw();
      break;

    case Type_Checkbox:
      checkboxes[*cursor].draw();
      break;

    case Type_Action:
      actions[*cursor].draw();
      break;

    default:
      break;
    }

    (*cursor)++;
    ImGui::Dummy(ImVec2(0, INPUT_GAP));
  }
}
