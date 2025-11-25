#include "tool_button.hpp"
#include "webgpu/webgpu.h"

Widget::ToolButtonShape::ToolButtonShape(WGPUTextureView view,
                                         const TextureAtlasRegion *region)
    : sprite(view, region) {}
