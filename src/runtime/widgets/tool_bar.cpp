#include "tool_bar.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/ui_sprite.h"
#include "webgpu/webgpu.h"

Widget::ToolBarShape::ToolBarShape(WGPUTextureView view) {

  this->view = view;

  bg_sprite = ui_sprite(UISprite_ToolBar);

  const float scale = context_dpi();

  bg_start =
      ImVec2(scale * (context_width() / 2.0f - bg_sprite->size[0] / 2.0f),
             scale * (context_height() - bg_sprite->size[1]));

  bg_end = ImVec2(bg_start[0] + scale * bg_sprite->size[0],
                  scale * context_height());
}

void Widget::ToolBarShape::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  dl->AddImage((ImTextureRef)view, bg_start, bg_end,
               im_vec2((float *)bg_sprite->uv0),
               im_vec2((float *)bg_sprite->uv1));
}
