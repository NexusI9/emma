#ifndef _WIDGET_TOOL_BUTTON_H_
#define _WIDGET_TOOL_BUTTON_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/include/texture.h"
#include "webgpu/webgpu.h"
#include <imgui/imgui.h>

namespace Widget {

class ToolButtonShape {

public:
  ToolButtonShape(WGPUTextureView, const TextureAtlasRegion *);
  Component::Sprite sprite;
  

private:
};

} // namespace Widget

#endif
