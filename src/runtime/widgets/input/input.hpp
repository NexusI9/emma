#ifndef _WIDGET_INPUT_H_
#define _WIDGET_INPUT_H_

#include "nkengine/include/list.h"
#include "runtime/solutions/module/compounds/action.h"
#include "runtime/solutions/module/compounds/reward.h"
#include "runtime/solutions/module/core.h"
#include "runtime/widgets/core.hpp"
#include <stdint.h>

namespace Widget {

namespace Input {

typedef enum {
  Type_Slider,
  Type_Toggle,
  Type_Segment,
  Type_Amount,
  Type_Combobox,
  Type_Checkbox,
  Type_Action,
  Type_COUNT,
} Type;

typedef struct {
  const char *label;
  float min;
  float max;
  float step;
  float multiplier;
  float *value;
} SliderParams;

typedef struct {
  const char *label;
  bool *active;
} ToggleParams;

typedef struct {
  const char *label;
  const char **items;
  uint32_t count;
  int *selected;
} SegmentParams;

typedef struct {
  const char *label;
  float min;
  float max;
  float step;
  float multiplier;
  float *value;
} AmountParams;

typedef struct {
  const char *label;
  const char **items;
  uint32_t count;
  int *selected;
} ComboboxParams;

typedef struct {
  const char *label;
  bool *active;
} CheckboxParams;

// Action Input Component will automatically map the input based on the
// provided solution
typedef struct {
  SolutionModuleAction *handle;
} ActionParams;

typedef struct {
  Type type;

  union {
    SliderParams slider;
    ToggleParams toggle;
    SegmentParams segment;
    AmountParams amount;
    ComboboxParams combobox;
    CheckboxParams checkbox;
    ActionParams action;
  };

} Params;

static const uint8_t INPUT_LIST_CAPACITY = 32;
typedef STATIC_LIST(Params, INPUT_LIST_CAPACITY) List;
} // namespace Input

} // namespace Widget

#endif
