#include "connector.hpp"
#include "runtime/manager/viewport.h"
#include "runtime/node/connector.h"

#include "nkengine/include/gui.hpp"
#include "runtime/node/connector_handle.h"
#include <imgui/imgui.h>

void Widget::ConnectorShape::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  dl->PathClear();

  const ConnectorHandle *h0 = connector_get_start_handle(node);
  const ConnectorHandle *h1 = connector_get_end_handle(node);

  vec2 start, end;
  connector_handle_get_position(h0, start);
  connector_handle_get_position(h1, end);

  vp2(start, start);
  vp2(end, end);

  dl->PathLineTo(im_vec2(start));

  const vec2 *corners = connector_get_corners(node);
  for (uint8_t i = 0; i < CONNECTOR_HANDLE_COUNT; i++)
    dl->PathLineTo(ImVec2(vpx(corners[i][0]), vpy(corners[i][1])));

  dl->PathLineTo(im_vec2(end));

  dl->PathStroke(im_color(node->color), 0, node->thickness);
}
