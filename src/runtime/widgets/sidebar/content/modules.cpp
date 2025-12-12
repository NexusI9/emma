#include "modules.hpp"
#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/module.h"
#include "runtime/manager/theme.h"
#include "runtime/widgets/sidebar/content/content.hpp"
#include "runtime/widgets/utils.hpp"
#include <cstdio>
#include <cstring>

Widget::SideBar::Content::Modules::Component::Component(Gui *gui)
    : ::Widget::SideBar::Content::Component(gui) {
  layout();
}

void Widget::SideBar::Content::Modules::Component::layout() {

  // clang-format off
  const uint8_t label_len = 14;
  const float thumb_base_y = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_4));
  const float label_gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Small));
  const float padding = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Small));
  const ImVec2 gap = ImVec2(
			  gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_2)),
			  gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_3))
			  );
  // clang-format on

  for (size_t i = 0; i < ModuleType_COUNT; i++) {
    const TextureAtlasRegion *region = get_module((ModuleType)i);

    float w = region->size[0];
    float h = region->size[1];

    // float scale = (h > w) ? cell_size.y / h : cell_size.x / w;
    float scale_x = cell_size.x / w;
    float scale_y = cell_size.y / h;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;

    float final_w = w * scale;
    float final_h = h * scale;

    int col = i % 3;
    int row = i / 3;

    frames_p0[i] = ImVec2(col * (cell_size.x + gap.x),
                          row * (cell_size.y + gap.y) + thumb_base_y);

    frames_p1[i] =
        ImVec2(frames_p0[i].x + cell_size.x, frames_p0[i].y + cell_size.y);

    // center the module within frame
    modules_position[i] =
        ImVec2(frames_p0[i].x + padding + (cell_size.x - final_w) * 0.5f,
               frames_p0[i].y + padding + (cell_size.y - final_h) * 0.5f);

    modules_size[i] = ImVec2(final_w - 2 * padding, final_h - 2 * padding);

    // place label below
    labels_position[i] = ImVec2(frames_p0[i].x, frames_p1[i].y + label_gap);

    // trim text
    if (strlen(region->label) >= label_len - 3) {
      name_t trim;
      snprintf(trim, label_len - 3, "%s", region->label);
      snprintf(labels[i], label_len, "%s...", trim);
    } else {
      snprintf(labels[i], label_len, "%s", region->label);
    }
  }
}

void Widget::SideBar::Content::Modules::Component::update() {

  ImVec2 win_pos = ImGui::GetCursorPos();

  for (size_t i = 0; i < ModuleType_COUNT; i++) {

    if (!active_thumbnail && ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
        ImGui::IsMouseHoveringRect(
            ImVec2(frames_p0[i].x + win_pos.x, frames_p0[i].y + win_pos.y),
            ImVec2(frames_p1[i].x + win_pos.x, frames_p1[i].y + win_pos.y))) {

      thumbnail_index = (ModuleType)i;
      active_thumbnail = get_module(thumbnail_index);
      mouse_init_pos = ImGui::GetMousePos();

      for (uint8_t j = 0; j < drag_begin_callbacks.count; j++)
        drag_begin_callbacks.entries[j].callback(
            active_thumbnail, thumbnail_index,
            drag_begin_callbacks.entries[j].data);
    }
  }

  if (active_thumbnail)
    for (uint8_t j = 0; j < drag_callbacks.count; j++)
      drag_callbacks.entries[j].callback(active_thumbnail, thumbnail_index,
                                         ImGui::GetMousePos(),
                                         drag_callbacks.entries[j].data);

  if (active_thumbnail && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {

    for (uint8_t j = 0; j < drag_end_callbacks.count; j++)
      drag_end_callbacks.entries[j].callback(
          active_thumbnail, thumbnail_index, ImGui::GetMousePos(),
          drag_end_callbacks.entries[j].data);

    active_thumbnail = nullptr;
  }
}

void Widget::SideBar::Content::Modules::Component::render() {

  ImGui::SetWindowFontScale(1.3);
  ImGui::Text("Modules");
  ImGui::SetWindowFontScale(1);
  ImGui::Dummy(
      ImVec2(0, gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_3))));

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 win_pos = ImGui::GetWindowPos();

  for (size_t i = 0; i < ModuleType_COUNT; i++) {

    const TextureAtlasRegion *region = get_module((ModuleType)i);

    // 3xN grid

    // background frame
    dl->AddRectFilled(
        ImVec2(frames_p0[i].x + win_pos.x, frames_p0[i].y + win_pos.y),
        ImVec2(frames_p1[i].x + win_pos.x, frames_p1[i].y + win_pos.y),
        emma_im_color(ThemeEmmaColor_Surface_Base), frame_rounding);

    // thumbnail
    ImGui::SetCursorPos(modules_position[i]);
    ImGui::Image((ImTextureRef)module_view, modules_size[i],
                 im_vec2(region->uv0), im_vec2(region->uv1));

    ImGui::SetCursorPos(labels_position[i]);
    ImGui::Text("%s", labels[i]);
  }

  if (active_thumbnail) {

    dl = ImGui::GetForegroundDrawList();
    ImVec2 mouse = ImGui::GetMousePos();
    ImVec2 drag_offset =
        ImVec2(mouse.x - mouse_init_pos.x, mouse.y - mouse_init_pos.y);

    dl->AddImage(
        (ImTextureRef)module_view,
        // p0
        ImVec2(modules_position[thumbnail_index].x + win_pos.x + drag_offset.x,
               modules_position[thumbnail_index].y + win_pos.y + drag_offset.y),
        // p1
        ImVec2(modules_position[thumbnail_index].x +
                   modules_size[thumbnail_index].x + win_pos.x + drag_offset.x,
               modules_position[thumbnail_index].y +
                   modules_size[thumbnail_index].y + win_pos.y + drag_offset.y),
        // uvs
        im_vec2(active_thumbnail->uv0), im_vec2(active_thumbnail->uv1));
  }
}
