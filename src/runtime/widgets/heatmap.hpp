#ifndef _EMMA_WIDGET_HEAT_MAP_H_
#define _EMMA_WIDGET_HEAT_MAP_H_

#include "runtime/node/heatmap.h"
namespace Widget {

class HeatmapShape {

public:
  HeatmapShape(Heatmap *);
  void draw();

private:
  Heatmap *node;

  
};

} // namespace Widget

#endif
