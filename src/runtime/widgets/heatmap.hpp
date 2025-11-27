#ifndef _EMMA_WIDGET_HEAT_MAP_H_
#define _EMMA_WIDGET_HEAT_MAP_H_

#include "runtime/node/heatmap.h"
#include <imgui/imgui.h>
#include <webgpu/webgpu.h>

namespace Widget {

class HeatmapShape {

public:
  HeatmapShape() {}
  HeatmapShape(Heatmap *);

  void draw();
  void compute_offline(WGPUCommandEncoder);
  const char *get_label() { return node->label; }
  const char *get_axis_0() { return node->axes[0]; }
  const char *get_axis_1() { return node->axes[1]; }

private:
  Heatmap *node;
  ImVec2 init_offset;
  void compute_render_pass(WGPUCommandEncoder);
  void compute_blur_pass(WGPUCommandEncoder);
  void compute_remap_pass(WGPUCommandEncoder);
};

} // namespace Widget

#endif
