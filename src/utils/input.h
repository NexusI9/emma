#ifndef _UTILS_INPUT_H_
#define _UTILS_INPUT_H_

#include "nkengine/include/input.h"

static inline bool input_wheel_moving() {
  static const float wheel_move_threshold = 4;
  return fabsf(input_wheel_x()) >= wheel_move_threshold ||
         fabsf(input_wheel_y()) >= wheel_move_threshold;
}

#endif
