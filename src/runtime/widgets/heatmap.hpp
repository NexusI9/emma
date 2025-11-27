#ifndef _EMMA_WIDGET_HEAT_MAP_H_
#define _EMMA_WIDGET_HEAT_MAP_H_

#include "runtime/node/heatmap.h"
#include <webgpu/webgpu.h>

namespace Widget {

class HeatmapShape {

public:
  HeatmapShape(Heatmap *);
  void draw();
  void compute_offline(WGPUCommandEncoder);
  bool require_update = false;

private:
  Heatmap *node;

  void compute_render_pass(WGPUCommandEncoder);
  void compute_blur_pass(WGPUCommandEncoder);
  void compute_remap_pass(WGPUCommandEncoder);
};

} // namespace Widget

#endif
