#include "toolbar.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/ui_sprite.h"
#include "webgpu/webgpu.h"

Widget::ToolBar::Component::Component(Gui *gui, WGPUTextureView view)
    : Widget(gui), background(view, ui_sprite(UISprite_Toolbar_Pad)),
      selector(view, ui_sprite(UISprite_Toolbar_Icon_Selector)),
      tools{
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Frame)),
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Cursor)),
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Persona)),
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Note)),
          Button::Component(view, ui_sprite(UISprite_Toolbar_Icon_Shape)),
      } {

  // cache window dimension and position
  size = ImVec2(gui_scale(gui, 513), gui_scale(gui, 88));
  position = ImVec2((float)gui_scale(gui, context_width()) / 2 - size.x / 2,
                    gui_scale(gui, context_height()) - size.y);

  // cache content position
  background.set_position(
      ImVec2(position.x + size.x / 2.0f, position.y + size.y),
      GuiSpriteAnchor_BottomMiddle);

  compute_sprite_position();
  selector.set_position(selector_positions[0], GuiSpriteAnchor_BottomLeft);
}

void Widget::ToolBar::Component::compute_sprite_position() {

  const float tool_w = tools[0].sprite.region->size[0];
  const float sel_w = selector.region->size[0];
  const float spacing = -60.0f;
  const float N = TOOLS_COUNT - 1;

  const float total_width = N * tool_w + (N - 1) * spacing;
  const float start_x = (context_width() - total_width) * 0.5f;
  const float bottom_margin = 15.0f;

  for (uint8_t i = 0; i < TOOLS_COUNT; i++) {
    float x = gui_scale(gui, start_x + i * (tool_w + spacing));
    float y = gui_scale(gui, context_height() - bottom_margin);

    tools[i].sprite.set_position(
        ImVec2(x, y),
        GuiSpriteAnchor_BottomLeft // better for left-to-right alignment
    );

    // cache selector position
    const ImVec2 diff = ImVec2(
        (selector.region->size[0] - tools[i].sprite.region->size[0]) / 2,
        (selector.region->size[1] - tools[i].sprite.region->size[1]) / 2);

    selector_positions[i] = ImVec2(x - diff.x, y + diff.y);
  }
}

void Widget::ToolBar::Component::draw() {

  UI::DockedWindow().Begin("ToolBar Window", position, size);
  {
    background.draw();
    selector.draw();

    for (uint8_t i = 0; i < TOOLS_COUNT; i++) {

      if (tools[i].sprite.clicked(ImGuiMouseButton_Left)) {
        active_tool = i;
        selector.set_position(selector_positions[i],
                              GuiSpriteAnchor_BottomLeft);
        for (uint8_t j = 0; j < callbacks.count; j++)
          callbacks.entries[j].callback(i, callbacks.entries[j].data);
      }

      tools[i].sprite.draw();
    }
  }
  UI::DockedWindow().End();
}

StaticListStatus
Widget::ToolBar::Component::add_callback(on_tool_change callback, void *data) {

  ToolBar::UpdateCallback entry = {.callback = callback, .data = data};

  return stli_insert(callbacks.entries, TOOLBAR_CALLBACK_CAPACITY,
                     &callbacks.count, sizeof(ToolBar::UpdateCallback), &entry,
                     "Tool Bar Shape Callback List");
}
