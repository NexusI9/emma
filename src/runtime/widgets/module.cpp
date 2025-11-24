#include "module.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/module.h"
#include "runtime/manager/viewport.h"

#include <imgui/imgui.h>

void Widget::ModuleShape::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImVec2 p0 = ImVec2(vpx(node->world_position[0]), vpy(node->world_position[1]));
  ImVec2 p1 = ImVec2(vpx(node->world_position[0] + node->size[0]),
                     vpy(node->world_position[1] + node->size[1]));

  dl->AddRect(p0, p1, ImColor(255, 0, 0, 255), 2.0f, 0, 4.0f);
  // dl->AddImage((ImTextureRef)module_view(), p0, p1, im_vec2(node->uv0),
  //              im_vec2(node->uv1));
}
