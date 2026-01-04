#include "segment.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include <imgui/imgui_internal.h>

void Widget::Segment::Component::layout() {

  RADIUS = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
  GAP = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  // set label
  ImVec2 label_size = ImGui::CalcTextSize(label);
  label_p = ImVec2(0, 0);

  // set buttons
  const ImVec2 BUTTON_SIZE =
      ImVec2(size.x / count,
             gui_scale(gui, emma_size(ThemeEmmaSize_Height_Input_Small)));

  const int BUTTON_BASE_Y = label_size.y + GAP;

  for (uint8_t i = 0; i < count; i++) {

    Button *b = &buttons[i];

    b->label = items[i];
    b->radius = RADIUS;

    // center label
    ImVec2 label_size = ImGui::CalcTextSize(b->label);
    b->label_p =
        ImVec2(BUTTON_SIZE.x * i + BUTTON_SIZE.x / 2 - label_size.x / 2,
               BUTTON_BASE_Y + BUTTON_SIZE.y / 2 - label_size.y / 2);

    b->p0 = ImVec2(i * BUTTON_SIZE.x, BUTTON_BASE_Y);
    b->p1 = ImVec2((i + 1) * BUTTON_SIZE.x, BUTTON_BASE_Y + BUTTON_SIZE.y);

    if (i == 0)
      b->flags =
          ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomLeft;
    else if (i == count - 1)
      b->flags = ImDrawFlags_RoundCornersTopRight |
                 ImDrawFlags_RoundCornersBottomRight;
    else
      b->flags = ImDrawFlags_RoundCornersNone;
  }
}

void Widget::Segment::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();
  ImGui::ItemSize(size);

  dl->AddText(im_vec2_add(label_p, origin), LABEL_COLOR, label);

  for (uint8_t i = 0; i < count; i++) {

    Button *b = &buttons[i];

    if (*selected == i) {

      dl->AddImageRounded((ImTextureRef)view, im_vec2_add(b->p0, origin),
                          im_vec2_add(b->p1, origin),
                          ImVec2(button_gradient.region->uv0[0],
                                 button_gradient.region->uv0[1]),
                          ImVec2(button_gradient.region->uv1[0],
                                 button_gradient.region->uv1[1]),
                          ImColor(255, 255, 255, 255), RADIUS, b->flags);

    } else {

      dl->AddRect(im_vec2_add(b->p0, origin), im_vec2_add(b->p1, origin),
                  STROKE_COLOR, RADIUS, b->flags);
    }

    dl->AddText(im_vec2_add(b->label_p, origin), LABEL_COLOR, b->label);
  }
}
