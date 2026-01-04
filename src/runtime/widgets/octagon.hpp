#ifndef _WIDGET_OCTAGON_H_
#define _WIDGET_OCTAGON_H_

#include "runtime/manager/viewport.h"
#include "runtime/node/octagon.h"
#include <imgui/imgui.h>

namespace Widget {

namespace Octagon {

typedef float (*transform_callback)(const float);

static inline float no_transform(const float value) { return value; }

class Component {

public:
  Component(::Octagon *node) : node(node) {}

  // draw fixed, according to window/ cursor position
  void draw(ImVec2 origin = ImGui::GetCursorScreenPos(),
            transform_callback transform_x = no_transform,
            transform_callback transform_y = no_transform,
            bool display_label = true);

  // draw for viewport, absolute position with viewport wheel movement
  void draw_viewport();

  ::Octagon *get_node() { return node; }

private:
  ::Octagon *node;

  void draw_labels(ImDrawList *draw_list, ImVec2 origin = ImVec2(0, 0),
                   transform_callback transform_x = no_transform,
                   transform_callback transform_y = no_transform);

  void draw_outer_gradient(ImDrawList *draw_list, ImVec2 origin = ImVec2(0, 0),
                           transform_callback transform_x = no_transform,
                           transform_callback transform_y = no_transform);

  void draw_inner_shape(ImDrawList *draw_list, ImVec2 origin = ImVec2(0, 0),
                        transform_callback transform_x = no_transform,
                        transform_callback transform_y = no_transform);

  ImU32 vertex_color_rgb(const int);
  ImU32 vertex_color(const int);
};

} // namespace Octagon
} // namespace Widget

#endif
