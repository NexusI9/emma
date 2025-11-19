#include "octagon.hpp"

#include "nkengine/include/gui.hpp"
#include <imgui/imgui.h>

ImU32 Widget::OctagonShape::vertex_color(const int vertex) {

  static const ImU32 colors[] = {
      IM_COL32(69, 251, 255, 255),  IM_COL32(112, 0, 255, 255),
      IM_COL32(193, 0, 173, 255),   IM_COL32(249, 0, 72, 255),
      IM_COL32(249, 139, 72, 255),  IM_COL32(255, 225, 21, 255),
      IM_COL32(216, 255, 154, 255), IM_COL32(131, 255, 63, 255),
  };

  return colors[vertex];
}

// Calculate interpolated color based on segment index (0 to 7)
// This creates the color gradient around the perimeter
ImU32 Widget::OctagonShape::vertex_color_rgb(const int vertex) {

  float t =
      (float)vertex / OCTAGON_VERTEX_COUNT; // t goes from 0.0 to approx 0.875

  // Simple gradient: Red -> Green -> Blue -> Red
  if (t < 0.33f) {
    // Red to Green (0.0 to 0.33)
    float t_local = t * 3.0f;
    return IM_COL32(255 * (1.0f - t_local), 255 * t_local, 0, 255);
  } else if (t < 0.66f) {
    // Green to Blue (0.33 to 0.66)
    float t_local = (t - 0.33f) * 3.0f;
    return IM_COL32(0, 255 * (1.0f - t_local), 255 * t_local, 255);
  } else {
    // Blue back to Red (0.66 to 1.0 - wraps around)
    float t_local = (t - 0.66f) * 3.0f;
    return IM_COL32(255 * t_local, 0, 255 * (1.0f - t_local), 255);
  }
}

void Widget::OctagonShape::draw_labels(ImDrawList *draw_list) {

  for (int i = 0; i < OCTAGON_VERTEX_COUNT; i++) {
    // Calculate position
    ImVec2 vert_pos =
        ImVec2(node->outer_vertices[i][0], node->outer_vertices[i][1]);
    ImVec2 label_pos =
        ImVec2(node->label_coordinates[i][0], node->label_coordinates[i][1]);

    draw_list->AddText(label_pos, ImColor(255, 255, 255, 255), node->labels[i]);
    draw_list->AddCircleFilled(vert_pos, 3.0f, IM_COL32(255, 255, 255, 255));
  }
}

void Widget::OctagonShape::draw_outer_gradient(ImDrawList *draw_list) {

  static const int num_segments = OCTAGON_VERTEX_COUNT;
  static const int num_vertices =
      num_segments + 1; // 8 outer points + 1 center point
  static const int num_indices =
      num_segments * 3; // 8 triangles * 3 indices each

  // Define the colors (using ImU32 format: ABGR by default in ImGui)
  // You can use ImGui::GetColorU32() to convert common formats
  const ImU32 COLOR_CENTER = IM_COL32(255, 255, 255, 255); // White (Center)

  // We'll calculate the outer colors using a function for smooth
  // interpolation

  // Allocate space in the draw list buffers
  draw_list->PrimReserve(num_indices, num_vertices);

  // --- Vertex Generation ---

  // a. Center Vertex (Vertex 0)
  ImDrawVert *center_vtx = draw_list->_VtxWritePtr;
  center_vtx->pos = ImVec2(node->position[0], node->position[1]);
  center_vtx->uv = ImGui::GetFontTexUvWhitePixel(); // Standard texture
                                                    // coordinate
  center_vtx->col = COLOR_CENTER;

  // Advance the pointer to the start of the outer vertices
  draw_list->_VtxWritePtr++;

  // b. Outer Vertices (Vertices 1 to 8)
  for (int i = 0; i < num_segments; i++) {
    // Calculate position
    float angle = i * (2.0f * GLM_PI / num_segments);
    ImVec2 vtx_pos =
        ImVec2(node->outer_vertices[i][0], node->outer_vertices[i][1]);

    ImU32 vtx_color = vertex_color(i);

    // Populate the vertex structure
    ImDrawVert *vtx = draw_list->_VtxWritePtr;
    vtx->pos = vtx_pos;
    vtx->uv = ImGui::GetFontTexUvWhitePixel();
    vtx->col = vtx_color;

    // Advance the vertex pointer
    draw_list->_VtxWritePtr++;
  }

  // --- Index Generation (Triangles) ---

  // The center vertex index is 0. Outer vertices are 1 to 8.
  const ImDrawIdx center_idx = draw_list->_VtxCurrentIdx;

  for (int i = 0; i < num_segments; i++) {
    ImDrawIdx *idx = draw_list->_IdxWritePtr;

    // Define the three vertices for the triangle fan
    // Triangle = (Center, Current Outer, Next Outer)

    idx[0] = center_idx;           // Center vertex (Index 0)
    idx[1] = center_idx + (i + 1); // Current outer vertex (Index 1 to 8)

    // Next outer vertex (wraps around from 8 back to 1)
    idx[2] = center_idx + ((i + 1) % num_segments) + 1;

    // Advance the index pointer
    draw_list->_IdxWritePtr += 3;
  }

  // Update the index base for the next primitive in the draw list
  draw_list->_VtxCurrentIdx += num_vertices;
}

void Widget::OctagonShape::draw() {

  ImDrawList *draw_list = ImGui::GetForegroundDrawList();

  draw_outer_gradient(draw_list);
  draw_list->AddConvexPolyFilled((ImVec2 *)node->inner_vertices,
                                 OCTAGON_VERTEX_COUNT,
                                 im_color(node->inner_color));
  draw_labels(draw_list);
}
