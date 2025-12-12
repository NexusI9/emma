#ifndef _WIDGET_TOOl_BAR_H_
#define _WIDGET_TOOl_BAR_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/include/texture.h"
#include "runtime/widgets/toolbar_button.hpp"
#include "webgpu/webgpu.h"
#include <imgui/imgui.h>

namespace Widget {

namespace ToolBar {

typedef void (*on_tool_change)(const uint8_t, void *);

typedef struct {
  on_tool_change callback;
  void *data;
} UpdateCallback;

class Component {

public:
  Component(WGPUTextureView view);
  void update();
  void render();
  StaticListStatus add_callback(on_tool_change, void *);

private:
  ::Component::Sprite background, selector;

  static constexpr uint8_t TOOLS_COUNT = 5;
  uint8_t active_tool = 0;
  Button::Component tools[TOOLS_COUNT];
  ImVec2 selector_positions[TOOLS_COUNT];

  static constexpr uint8_t TOOLBAR_CALLBACK_CAPACITY = 128;
  STATIC_LIST(UpdateCallback, TOOLBAR_CALLBACK_CAPACITY) callbacks;
};

} // namespace ToolBar
} // namespace Widget

#endif
