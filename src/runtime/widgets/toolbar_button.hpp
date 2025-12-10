#ifndef _WIDGET_TOOL_BUTTON_H_
#define _WIDGET_TOOL_BUTTON_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/include/texture.h"
#include "webgpu/webgpu.h"
#include <imgui/imgui.h>

namespace Widget {

namespace ToolBar {

namespace Button {
class Component {

public:
  Component(WGPUTextureView, const TextureAtlasRegion *);
  ::Component::Sprite sprite;

private:
};

} // namespace Button
} // namespace ToolBar
} // namespace Widget

#endif
