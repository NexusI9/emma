#ifndef _WIDGET_UTILS_H_
#define _WIDGET_UTILS_H_

#include "runtime/manager/viewport.h"
#include <imgui/imgui.h>

static inline ImVec2 vp_im2(ImVec2 value) {
  return ImVec2(vpx(value.x), vpy(value.y));
}

static inline ImVec2 vp_im2_scene(ImVec2 value) {
  return ImVec2(vpx_scene(value.x), vpy_scene(value.y));
}

#endif
