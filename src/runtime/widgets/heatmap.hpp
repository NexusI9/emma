#ifndef _EMMA_WIDGET_HEAT_MAP_H_
#define _EMMA_WIDGET_HEAT_MAP_H_

#include "nkengine/include/gui.h"
#include "nkengine/include/gui.hpp"
#include "runtime/node/heatmap.h"
#include <imgui/imgui.h>
#include <webgpu/webgpu.h>

namespace Widget {

class HeatmapShape {

public:
  HeatmapShape(Gui *gui, Heatmap *node)
      : gui(gui), node(node),
        gradient(
            gui_scale_im_vec2(gui, gradient_anchor),
            gui_scale_im_vec2(gui, ImVec2(gradient_anchor.x + gradient_width,
                                          gradient_anchor.y + gradient_height)),
            node->color_map.colors, node->color_map.count) {}

  void draw();
  void compute_offline(WGPUCommandEncoder);
  const char *get_label() { return node->label; }
  const char *get_axis_0() { return node->axes[0]; }
  const char *get_axis_1() { return node->axes[1]; }

private:
  Gui *gui;
  Heatmap *node;

  static constexpr ImVec2 gradient_anchor = ImVec2(40, 200);
  static constexpr int gradient_width = 30;
  static constexpr int gradient_height = 350;

  UI::GradientRect gradient;

  ImVec2 init_offset;

  void compute_render_pass(WGPUCommandEncoder);
  void compute_blur_pass(WGPUCommandEncoder);
  void compute_remap_pass(WGPUCommandEncoder);
  void draw_gradient(ImDrawList *);
  void draw_gradient_legend(ImDrawList *, ImVec2, ImVec2, const char *);
};

} // namespace Widget

#endif
