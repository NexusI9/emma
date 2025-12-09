#ifndef _WIDGET_UTILS_H_
#define _WIDGET_UTILS_H_

#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/viewport.h"
#include <imgui/imgui.h>

static inline ImVec2 vp_im2(ImVec2 value) {
  return ImVec2(vpx(value.x), vpy(value.y));
}

static inline ImVec2 vp_im2_scene(ImVec2 value) {
  return ImVec2(vpx_scene(value.x), vpy_scene(value.y));
}

static inline ImColor emma_im_color(const ThemeEmmaColor color) {
  const float *src = emma_color(color);
  return ImColor(src[0], src[1], src[2], src[3]);
}

#endif
