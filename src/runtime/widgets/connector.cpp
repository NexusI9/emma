#include "connector.hpp"
#include "runtime/manager/viewport.h"
#include "runtime/node/connector.h"

#include "nkengine/include/gui.hpp"
#include "runtime/node/frame.h"
#include "runtime/node/connector_handle.h"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/utils.hpp"
#include <imgui/imgui.h>
#include <stdint.h>

void Widget::ConnectorShape::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  vec2 vp_start, vp_end;
  glm_vec2_copy(node->handles[0].position, vp_start);
  glm_vec2_copy(node->handles[1].position, vp_end);

  dl->PathClear();

  vp2(vp_start, vp_start);
  vp2(vp_end, vp_end);

  dl->PathLineTo(im_vec2(vp_start));

  const vec2 *corners = connector_get_corners(node);
  for (uint8_t i = 0; i < FRAME_CONNECTOR_HANDLE_COUNT; i++)
    dl->PathLineTo(ImVec2(vpx(corners[i][0]), vpy(corners[i][1])));

  dl->PathLineTo(im_vec2(vp_end));

  dl->PathStroke(im_color(node->color), 0, node->thickness);

  // draw arrow
  int dir =
      node->handles[0].position[0] < node->handles[1].position[0] ? 1 : -1;

  dl->AddLine(im_vec2(vp_end),
              ImVec2(vp_end[0] - gui_scale(gui, arrow_dist) * dir,
                     vp_end[1] - gui_scale(gui, arrow_dist)),
              im_color(node->color), node->thickness);

  dl->AddLine(im_vec2(vp_end),
              ImVec2(vp_end[0] - gui_scale(gui, arrow_dist) * dir,
                     vp_end[1] + gui_scale(gui, arrow_dist)),
              im_color(node->color), node->thickness);
}
