#ifndef _WIDGET_INPUT_ACTION_H_
#define _WIDGET_INPUT_ACTION_H_

#include "runtime/solutions/module/compounds/action.h"
#include "runtime/solutions/module/compounds/reward.h"
#include "runtime/solutions/module/core.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/input.hpp"

namespace Widget {

namespace Input {

namespace Action {

class Component : public Widget {

public:
  Component() : Widget(nullptr) {}

  typedef enum {
    Status_Success,
    Status_ValidationFailed,
  } Status;

  void init(Gui *);
  Status validate() const;
  void layout();
  void draw();

  void set_label(const char *label) { this->label = label; }
  void set_role(CompoundModuleActionRole *role) {
    this->role.segment.selected = (int *)role;
  }

  void set_active(bool *active) { this->active.checkbox.active = active; }

  void set_reward_type(CompoundModuleRewardType *type) {
    this->reward.type.combobox.selected = (int *)type;
  }

  void set_reward_probability(float *probability) {
    this->reward.probability.amount.value = probability;
  };

  void set_reward_amount_type(CompoundModuleRewardAmountType *type) {
    this->reward.amount.type.combobox.selected = (int *)type;
  }

  void set_reward_amount_fixed(uint32_t *value) {
    this->reward.amount.value.amount.value = (float *)value;
  }

  void set_reward_amount_range(uint32_t *min, uint32_t *max) {
    this->reward.amount.min.amount.value = (float *)min;
    this->reward.amount.max.amount.value = (float *)max;
  }

  void set_reward_frequency_quota(uint32_t *value) {
    this->reward.frequency.quota.amount.value = (float *)value;
  }

  void set_reward_frequency_interval(uint32_t *value) {
    this->reward.frequency.interval.amount.value = (float *)value;
  }

  void set_reward_frequency_repeat(uint32_t *value) {
    this->reward.frequency.repeat.amount.value = (float *)value;
  }

  void set_reward_frequency_forever(bool *value) {
    this->reward.frequency.forever.checkbox.active = value;
  }

  void set_reward_frequency_unit(CompoundModuleRewardTimeUnit *unit) {
    this->reward.frequency.unit.combobox.selected = (int *)unit;
  }

  void set_reward_time_limit_active(bool *value) {
    this->reward.time_limit.active.checkbox.active = value;
  }

  void set_reward_time_limit_amount(uint32_t *value) {
    this->reward.time_limit.active.amount.value = (float *)value;
  }

  void set_reward_time_limit_unit(CompoundModuleRewardTimeUnit *unit) {
    this->reward.time_limit.unit.combobox.selected = (int *)unit;
  }

private:
  const char *label;
  Input::Descriptor role;
  Input::Descriptor active;

  struct {
    Input::Descriptor type;
    Input::Descriptor probability;

    struct {
      Input::Descriptor type;
      Input::Descriptor value;
      Input::Descriptor min;
      Input::Descriptor max;
    } amount;

    struct {
      Input::Descriptor quota;
      Input::Descriptor interval;
      Input::Descriptor repeat;
      Input::Descriptor forever;
      Input::Descriptor unit;
    } frequency;

    struct {
      Input::Descriptor active;
      Input::Descriptor amount;
      Input::Descriptor unit;
    } time_limit;

  } reward;
};

} // namespace Action
} // namespace Input

} // namespace Widget

#endif
