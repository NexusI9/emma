#ifndef _WIDGET_TOOl_BAR_H_
#define _WIDGET_TOOl_BAR_H_

#include "webgpu/webgpu.h"
#include "nkengine/include/texture.h"
#include <imgui/imgui.h>

namespace Widget {

class ToolBarShape {

public:
  ToolBarShape(WGPUTextureView view);
  void draw();

private:
  WGPUTextureView view;
  const TextureAtlasRegion* bg_sprite;
  ImVec2 bg_start, bg_end;
};

} // namespace Widget

#endif
