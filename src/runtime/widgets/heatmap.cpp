#include "heatmap.hpp"
#include "runtime/node/heatmap.h"

Widget::HeatmapShape::HeatmapShape(Heatmap *map) { node = map; }

void Widget::HeatmapShape::draw() {}
