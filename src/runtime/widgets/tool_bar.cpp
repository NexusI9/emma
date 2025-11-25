#include "tool_bar.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/ui_sprite.h"
#include "webgpu/webgpu.h"

Widget::ToolBarShape::ToolBarShape(WGPUTextureView view)
    : bg_sprite(view, ui_sprite(UISprite_ToolBar)),
      tools{
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Frame)),
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Module)),
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Shape)),
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Note)),
          ToolButtonShape(view, ui_sprite(UISprite_ToolBar_Icon_Persona)),
      } {

  const float scale = context_dpi();

  bg_sprite.set_position(
      ImVec2(scale * context_width() / 2.0f, scale * context_height()),
      GuiSpriteAnchor_BottomMiddle);

  float tool_w = tools[0].sprite.region->size[0];
  float spacing = -60.0f;
  float N = TOOLS_COUNT - 1;

  float total_width = N * tool_w + (N - 1) * spacing;
  float start_x = (context_width() - total_width) * 0.5f;

  for (uint8_t i = 0; i < TOOLS_COUNT; i++) {
    float x = start_x + i * (tool_w + spacing);
    float y = context_height() - 20.0f;

    tools[i].sprite.set_position(
        ImVec2(x * scale, y * scale),
        GuiSpriteAnchor_BottomLeft // better for left-to-right alignment
    );
  }
}

void Widget::ToolBarShape::draw() {

  ImGuiViewport *vp = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(vp->Pos);
  ImGui::SetNextWindowSize(vp->Size);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  ImGui::Begin("ToolBar", NULL,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground |
                   ImGuiWindowFlags_NoBringToFrontOnFocus);
  bg_sprite.draw();
  for (uint8_t i = 0; i < TOOLS_COUNT; i++)
    tools[i].sprite.draw();

  ImGui::End();
  ImGui::PopStyleVar(2);
}
