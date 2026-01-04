#ifndef _WIDGET_INPUT_H_
#define _WIDGET_INPUT_H_

#include "nkengine/include/list.h"
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
  Type type;

  union {

    struct {
      const char *label;
      float min;
      float max;
      float step;
      float multiplier;
      float *value;
    } slider;

    struct {
      const char *label;
      bool *active;
    } toggle;

    struct {
      const char *label;
      const char **items;
      uint32_t count;
      int *selected;
    } segment;

    struct {
      const char *label;
      float min;
      float max;
      float step;
      float multiplier;
      float *value;
    } amount;

    struct {
      const char *label;
      const char **items;
      uint32_t count;
      int *selected;
    } combobox;

    struct {
      const char *label;
      bool *active;
    } checkbox;

    struct {
    } action;
  };

} Descriptor;

static const uint8_t INPUT_LIST_CAPACITY = 32;
typedef STATIC_LIST(Descriptor, INPUT_LIST_CAPACITY) List;
} // namespace Input

} // namespace Widget

#endif
