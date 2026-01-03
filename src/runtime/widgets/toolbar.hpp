#ifndef _WIDGET_TOOl_BAR_H_
#define _WIDGET_TOOl_BAR_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/include/texture.h"
#include "runtime/layouts/core.hpp"
#include "runtime/widgets/core.hpp"
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

class Component : public Widget, public Layout::Window {

public:
  Component(const char *, Gui *, WGPUTextureView);
  void draw();
  StaticListStatus add_callback(on_tool_change, void *);

private:
  static constexpr uint8_t TOOLS_COUNT = 4;

  ::Component::Sprite background, selector;

  ImVec2 size, position;
  ImVec2 selector_positions[TOOLS_COUNT];

  uint8_t active_tool = 0;
  Button::Component tools[TOOLS_COUNT];

  static constexpr uint8_t TOOLBAR_CALLBACK_CAPACITY = 128;
  STATIC_LIST(UpdateCallback, TOOLBAR_CALLBACK_CAPACITY) callbacks;

  void compute_sprite_position();
};

} // namespace ToolBar
} // namespace Widget

#endif
