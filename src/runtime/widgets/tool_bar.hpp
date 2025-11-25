#ifndef _WIDGET_TOOl_BAR_H_
#define _WIDGET_TOOl_BAR_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/include/texture.h"
#include "runtime/widgets/tool_button.hpp"
#include "webgpu/webgpu.h"
#include <imgui/imgui.h>

namespace Widget {

class ToolBarShape {

public:
  ToolBarShape(WGPUTextureView view);
  void draw();

private:
  Component::Sprite bg_sprite;
  static constexpr uint8_t TOOLS_COUNT = 5;
  ToolButtonShape tools[TOOLS_COUNT];
};

} // namespace Widget

#endif
