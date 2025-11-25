#include "heatmap.hpp"
#include "nkengine/include/context.h"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/module.hpp"
#include <imgui/imgui.h>

Widget::HeatmapShape::HeatmapShape(Heatmap *map) { node = map; }

void Widget::HeatmapShape::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  // TODO: cache vp size ?
  ImGuiViewport *vp = ImGui::GetMainViewport();

  dl->AddRectFilled(ImVec2(0, 0), vp->Size, im_color(node->background));

  // draw each modules
  for (size_t i = 0; i < *node->frames.length; i++) {
    Frame *parent_frame = allocator_frame_entry(node->frames.entries[i]);
    for (size_t j = 0; j < parent_frame->children.length; j++) {
      Frame *module = allocator_frame_entry(parent_frame->children.entries[j]);

      //float intensity = node->intensity_mapper(node, module, NULL);

      ModuleShape(module).draw_fill(ImColor(255, 0, 0, 255));
    }
  }
}
