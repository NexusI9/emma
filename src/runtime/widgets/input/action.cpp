#include "action.hpp"
#include "runtime/solutions/module/core.h"
#include <cstdio>

#define REQUIRE_PTR(p)                                                         \
  do {                                                                         \
    if ((p) == nullptr) {                                                      \
      fprintf(stderr, "%s property not set for Action", #p);                   \
      return Status_ValidationFailed;                                          \
    }                                                                          \
  } while (0)


void Widget::Input::Action::Component::init(Gui *gui) { this->gui = gui; }

/**
   Make sure all the components are set properly prior drawing.
 */
Widget::Input::Action::Component::Status
Widget::Input::Action::Component::validate() const {

  // Core
  REQUIRE_PTR(label);
  REQUIRE_PTR(role.segment.selected);
  REQUIRE_PTR(active.checkbox.active);

  // Reward – type & probability
  REQUIRE_PTR(reward.type.combobox.selected);
  REQUIRE_PTR(reward.probability.amount.value);

  // Reward – amount
  REQUIRE_PTR(reward.amount.type.combobox.selected);

  // Fixed OR Range is assumed to be handled by type,
  // but pointers must still be valid if used.
  if (reward.amount.value.amount.value) {
    REQUIRE_PTR(reward.amount.value.amount.value);
  } else {
    REQUIRE_PTR(reward.amount.min.amount.value);
    REQUIRE_PTR(reward.amount.max.amount.value);
  }

  // Reward – frequency
  REQUIRE_PTR(reward.frequency.quota.amount.value);
  REQUIRE_PTR(reward.frequency.interval.amount.value);
  REQUIRE_PTR(reward.frequency.repeat.amount.value);
  REQUIRE_PTR(reward.frequency.forever.checkbox.active);
  REQUIRE_PTR(reward.frequency.unit.combobox.selected);

  // Reward – time limit
  REQUIRE_PTR(reward.time_limit.active.checkbox.active);

  if (*reward.time_limit.active.checkbox.active) {
    REQUIRE_PTR(reward.time_limit.active.amount.value);
    REQUIRE_PTR(reward.time_limit.unit.combobox.selected);
  }

  return Status_Success;
}

void Widget::Input::Action::Component::layout() {}

void Widget::Input::Action::Component::draw() {}
