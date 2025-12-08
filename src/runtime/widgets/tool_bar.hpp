#ifndef _WIDGET_TOOl_BAR_H_
#define _WIDGET_TOOl_BAR_H_

#include "nkengine/include/gui.hpp"
#include "nkengine/include/texture.h"
#include "runtime/widgets/tool_button.hpp"
#include "webgpu/webgpu.h"
#include <imgui/imgui.h>

namespace Widget {

typedef void (*on_tool_change)(const uint8_t, void *);

typedef struct {
  on_tool_change callback;
  void *data;
} ToolBarShapeCallback;

class ToolBarShape {

public:
  ToolBarShape(WGPUTextureView view);
  void draw();
  StaticListStatus add_callback(on_tool_change, void *);

private:
  Component::Sprite background, selector;

  static constexpr uint8_t TOOLS_COUNT = 5;
  uint8_t active_tool = 0;
  ToolButtonShape tools[TOOLS_COUNT];
  ImVec2 selector_positions[TOOLS_COUNT];

  static constexpr uint8_t TOOLBAR_CALLBACK_CAPACITY = 128;
  struct {
    ToolBarShapeCallback entries[TOOLBAR_CALLBACK_CAPACITY];
    size_t count;
  } callbacks;
};

} // namespace Widget

#endif
