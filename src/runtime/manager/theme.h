#ifndef _EMMA_THEME_MANAGER_H_
#define _EMMA_THEME_MANAGER_H_

#include "resources/theme.emma.h"

static inline const float *emma_color(const ThemeEmmaColor color) {
  return THEME_EMMA_COLOR[color];
}

static inline const float emma_size(const ThemeEmmaSize size) {
  return THEME_EMMA_SIZE[size];
}




#endif
