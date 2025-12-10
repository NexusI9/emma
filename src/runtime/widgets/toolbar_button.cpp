#include "toolbar_button.hpp"
#include "webgpu/webgpu.h"

Widget::ToolBar::Button::Component::Component(WGPUTextureView view,
                                              const TextureAtlasRegion *region)
    : sprite(view, region) {}
