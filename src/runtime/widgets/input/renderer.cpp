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
      actions[*cursor].init(gui);
      map_action(input, &actions[*cursor]);
      actions[*cursor].validate();
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
      actions[*cursor].layout();
      break;

    default:
      break;
    }

    (*cursor)++;
  }
}

/**
   Since the action is kinda like a Set of Inputs, it would be quite cumbersombe
   to put all the parameters as argument. As a result we use a combination of
   API calls as well a validation to make sure everything is set properly before
   draw.

   This function is called right after the initialisation and before the layout.
   It acts as a more descriptive init().

   TODO: Currently we use many steps ton translate Solutions attributes into
   Inputs:

   Solution => Action => InputDescriptor => [ MAP ] => Input Component

   I feel like maybe the Action struct and Input Descriptor could be merged or
   something caue it feel very repetitive to map all the value.
   So streamline and simplify the whole thing.


 */
void Widget::Input::Renderer::Component::map_action(
    Input::Descriptor *descriptor, Action::Component *action) {

  // clang-format off
  action->set_label(descriptor->action.label);
  action->set_role(descriptor->action.role);
  action->set_active(descriptor->action.active);
  
  // reward
  action->set_reward_probability(descriptor->action.reward.probability);
  action->set_reward_type(descriptor->action.reward.type);
  
  // amount  
  action->set_reward_amount_type(descriptor->action.reward.amount.type);
  action->set_reward_amount_fixed(descriptor->action.reward.amount.value);
  action->set_reward_amount_type(descriptor->action.reward.amount.type);
  action->set_reward_amount_range(descriptor->action.reward.amount.range.min,
                                  descriptor->action.reward.amount.range.max);

  // frequency
  action->set_reward_frequency_quota(descriptor->action.reward.frequency.quota);
  action->set_reward_frequency_interval(descriptor->action.reward.frequency.interval);
  action->set_reward_frequency_repeat(descriptor->action.reward.frequency.repeat);
  action->set_reward_frequency_forever(descriptor->action.reward.frequency.forever);
  action->set_reward_frequency_unit(descriptor->action.reward.frequency.unit);

  // time limit
  action->set_reward_time_limit_active(descriptor->action.reward.time_limit.active);
  action->set_reward_time_limit_amount(descriptor->action.reward.time_limit.amount);
  action->set_reward_time_limit_unit(descriptor->action.reward.time_limit.unit);

  // clang-format on
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
