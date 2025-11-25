#include "tool_bar.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/ui_sprite.h"
#include "webgpu/webgpu.h"

Widget::ToolBarShape::ToolBarShape(WGPUTextureView view)
    : bg_sprite(view, ui_sprite(UISprite_ToolBar)),
      tools{
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Frame)),
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Module)),
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Persona)),
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Note)),
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Shape)),
      } {

  const float scale = context_dpi();

  bg_sprite.set_position(
      ImVec2(scale * context_width() / 2.0f, scale * context_height()),
      GuiSpriteAnchor_BottomMiddle);

  const float tool_w = tools[0].sprite.region->size[0];
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
  }
}

void Widget::ToolBarShape::draw() {

  bg_sprite.draw();
  for (uint8_t i = 0; i < TOOLS_COUNT; i++)
    tools[i].sprite.draw();
}
