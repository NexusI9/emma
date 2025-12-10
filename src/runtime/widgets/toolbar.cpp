#include "toolbar.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/ui_sprite.h"
#include "webgpu/webgpu.h"

Widget::ToolBar::Component::Component(WGPUTextureView view)
    : background(view, ui_sprite(UISprite_Toolbar)),
      selector(view, ui_sprite(UISprite_Toolbar_Icon_Selector)),
      tools{
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Frame)),
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Module)),
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Persona)),
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Note)),
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Shape)),
      } {

  const float scale = context_dpi();

  background.set_position(
      ImVec2(scale * context_width() / 2.0f, scale * context_height()),
      GuiSpriteAnchor_BottomMiddle);

  const float tool_w = tools[0].sprite.region->size[0];
  const float sel_w = selector.region->size[0];
  const float spacing = -60.0f;
  const float N = TOOLS_COUNT - 1;

  const float total_width = N * tool_w + (N - 1) * spacing;
  const float start_x = (context_width() - total_width) * 0.5f;
  const float bottom_margin = 15.0f;

  for (uint8_t i = 0; i < TOOLS_COUNT; i++) {
    float x = start_x + i * (tool_w + spacing);
    float y = context_height() - bottom_margin;

    tools[i].sprite.set_position(
        ImVec2(x * scale, y * scale),
        GuiSpriteAnchor_BottomLeft // better for left-to-right alignment
    );

    // cache selector position
    const ImVec2 diff = ImVec2(
        (selector.region->size[0] - tools[i].sprite.region->size[0]) / 2,
        (selector.region->size[1] - tools[i].sprite.region->size[1]) / 2);

    selector_positions[i] = ImVec2(x * scale - diff.x, y * scale + diff.y);
  }

  selector.set_position(selector_positions[0], GuiSpriteAnchor_BottomLeft);
}

void Widget::ToolBar::Component::update() {

  for (uint8_t i = 0; i < TOOLS_COUNT; i++) {
    if (tools[i].sprite.clicked(ImGuiMouseButton_Left)) {
      active_tool = i;
      selector.set_position(selector_positions[i], GuiSpriteAnchor_BottomLeft);
      for (uint8_t j = 0; j < callbacks.count; j++)
        callbacks.entries[j].callback(i, callbacks.entries[j].data);
    }
  }
}

void Widget::ToolBar::Component::render() {

  background.draw();
  selector.draw();

  for (uint8_t i = 0; i < TOOLS_COUNT; i++)
    tools[i].sprite.draw();
}

StaticListStatus
Widget::ToolBar::Component::add_callback(on_tool_change callback, void *data) {

  ToolBar::UpdateCallback entry = {.callback = callback, .data = data};

  return stli_insert(callbacks.entries, TOOLBAR_CALLBACK_CAPACITY,
                     &callbacks.count, sizeof(ToolBar::UpdateCallback), &entry,
                     "Tool Bar Shape Callback List");
}
