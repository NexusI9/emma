#include "frame.hpp"

#include "imgui/imgui.h"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/frame.h"
#include "runtime/widgets/connector_handle.hpp"

void Widget::FrameShape::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  // Frame rectangle
  ImVec2 p0 =
      ImVec2(vpx(node->world_position[0]), vpy(node->world_position[1]));
  ImVec2 p1 = ImVec2(vpx(node->world_position[0] + node->size[0]),
                     vpy(node->world_position[1] + node->size[1]));

  // Draw the core frame
  dl->AddRectFilled(p0, p1, im_color(node->background), 0.0f);
}

bool Widget::FrameShape::clickbox_hovered() {
  for (uint8_t i = 0; i < node->clickbox.count; i++)
    if (ImGui::IsMouseHoveringRect(
            ImVec2(vpx(node->clickbox.entries[i].p0[0]),
                   vpy(node->clickbox.entries[i].p0[1])),
            ImVec2(vpx(node->clickbox.entries[i].p1[0]),
                   vpy(node->clickbox.entries[i].p1[1]))))
      return true;

  return false;
}

bool Widget::FrameShape::boundbox_hovered() {
  return ImGui::IsMouseHoveringRect(
      ImVec2(vpx(node->world_position[0]), vpy(node->world_position[1])),
      ImVec2(vpx(node->end_point[0]), vpy(node->end_point[1])));
}

void Widget::FrameShape::draw_fill(ImColor color, const float scale) {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImVec2 p0 = ImVec2(vpx(node->world_position[0]) * scale,
                     vpy(node->world_position[1]) * scale);

  ImVec2 p1 = ImVec2(vpx(node->world_position[0] + node->size[0]) * scale,
                     vpy(node->world_position[1] + node->size[1]) * scale);

  dl->AddRectFilled(p0, p1, color);
}

void Widget::FrameShape::draw_texture() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImVec2 p0 =
      ImVec2(vpx(node->world_position[0]), vpy(node->world_position[1]));
  ImVec2 p1 = ImVec2(vpx(node->world_position[0] + node->size[0]),
                     vpy(node->world_position[1] + node->size[1]));

  if (node->parent != ID_UNDEFINED) {
    Frame *parent = allocator_frame_entry(node->parent);
    ImGui::PushClipRect(
        ImVec2(vpx(parent->world_position[0]), vpy(parent->world_position[1])),
        ImVec2(vpx(parent->end_point[0]), vpy(parent->end_point[1])), false);
  }

  dl->AddImage((ImTextureRef)texture_atlas_layer_view(&g_atlas,
                                                      TextureAtlasLayer_Module),
               p0, p1, im_vec2(node->uv0), im_vec2(node->uv1));

  if (node->parent != ID_UNDEFINED)
    ImGui::PopClipRect();
}

void Widget::FrameShape::draw_pod() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImVec2 p0 =
      ImVec2(vpx(node->world_position[0]), vpy(node->world_position[1]));
  ImVec2 p1 = ImVec2(vpx(node->world_position[0] + node->size[0]),
                     vpy(node->world_position[1] + node->size[1]));

  dl->AddImage(
      (ImTextureRef)texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
      p0, p1, im_vec2(node->uv0), im_vec2(node->uv1));

  for (size_t i = 0; i < node->children.length; i++) {
    Frame *child = allocator_frame_entry(node->children.entries[i]);
    FrameShape(child).draw_pod();
  }
}


