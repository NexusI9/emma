#include "slider.hpp"
#include "nkengine/vendor/imgui/imgui_internal.h"

void Widget::Slider::Component::layout() {

  SLIDER_SIZE = ImVec2(gui_scale(gui, 324), gui_scale(gui, 4));
  DOT_SIZE = gui_scale(gui, emma_size(ThemeEmmaSize_Icon_Medium));
  ROW_GAP = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  const int BASE_SLIDER_Y = label_p.y + ImGui::GetFontSize() + ROW_GAP;
  const int BAR_Y = BASE_SLIDER_Y + gui_scale(gui, 14);
  const int KNOB_Y = BASE_SLIDER_Y + gui_scale(gui, 3);

  {
    slider_p0.x = 0.0f;
    slider_p0.y = BAR_Y;

    slider_p1.x = SLIDER_SIZE.x;
    slider_p1.y = BAR_Y + SLIDER_SIZE.y;
  }

  slider_active_p0 = slider_p0;
  slider_active_p1 = slider_p1;

  knob.set_position(ImVec2(slider_active_p1.x, (float)KNOB_Y),
                    GuiSpriteAnchor_TopLeft);

  if (value)
    update_value(*value);

  item_size = ImVec2(SLIDER_SIZE.x, knob.get_end().y - label_p.y);
}

void Widget::Slider::Component::update_value(const float value) {

  slider_active_p1.x = SLIDER_SIZE.x * value;
  knob.set_position(ImVec2(slider_active_p1.x, knob.get_start().y),
                    GuiSpriteAnchor_TopMiddle);
}

void Widget::Slider::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImVec2 origin = ImGui::GetCursorScreenPos();
  ImGui::ItemSize(item_size);

  dl->AddText(im_vec2_add(label_p, origin), LABEL_COLOR, label);

  dl->AddRectFilled(im_vec2_add(slider_p0, origin),
                    im_vec2_add(slider_p1, origin), DEFAULT_BACKGROUND);
  dl->AddRectFilled(im_vec2_add(slider_active_p0, origin),
                    im_vec2_add(slider_active_p1, origin), ACTIVE_BACKGROUND);

  knob.draw_at(origin);
}
