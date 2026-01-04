#include "slider.hpp"
#include "nkengine/vendor/imgui/imgui_internal.h"

void Widget::Slider::Component::layout() {

  slider_size = ImVec2(gui_scale(gui, 324), gui_scale(gui, 4));
  dot_size = gui_scale(gui, emma_size(ThemeEmmaSize_Icon_Medium));
  row_gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));
  amount_gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Large));

  const int BASE_SLIDER_Y = label_p.y + ImGui::GetFontSize() + row_gap;
  const int BAR_Y = BASE_SLIDER_Y + gui_scale(gui, 14);
  const int KNOB_Y = BASE_SLIDER_Y + gui_scale(gui, 3);

  {
    slider_p0.x = 0.0f;
    slider_p0.y = BAR_Y;

    slider_p1.x = slider_size.x;
    slider_p1.y = BAR_Y + slider_size.y;
  }

  slider_active_p0 = slider_p0;
  slider_active_p1 = slider_p1;

  knob.set_position(ImVec2(slider_active_p1.x, (float)KNOB_Y),
                    GuiSpriteAnchor_TopLeft);

  amount_p = ImVec2(slider_active_p1.x + amount_gap, KNOB_Y);

  if (value)
    update_value(*value);

  size = ImVec2(slider_size.x, knob.get_end().y - label_p.y);
}

void Widget::Slider::Component::update_value(const float value) {

  slider_active_p1.x = slider_size.x * value;
  knob.set_position(ImVec2(slider_active_p1.x, knob.get_start().y),
                    value == 0.0f   ? GuiSpriteAnchor_TopLeft
                    : value == 1.0f ? GuiSpriteAnchor_TopRight
                                    : GuiSpriteAnchor_TopMiddle);

  switch (format) {

  case Format_Float:
    name_compose(str_value, "%.2f", value * multiplier);
    break;

  case Format_Integer:
    name_compose(str_value, "%d", (int)(value * multiplier));
    break;
  }
}

void Widget::Slider::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImVec2 origin = ImGui::GetCursorScreenPos();
  ImGui::ItemSize(size);

  dl->AddText(im_vec2_add(label_p, origin), LABEL_COLOR, label);

  ImGui::SetWindowFontScale(1.3);
  dl->AddText(im_vec2_add(amount_p, origin), LABEL_COLOR, str_value);
  ImGui::SetWindowFontScale(1);

  dl->AddRectFilled(im_vec2_add(slider_p0, origin),
                    im_vec2_add(slider_p1, origin), DEFAULT_BACKGROUND);
  dl->AddRectFilled(im_vec2_add(slider_active_p0, origin),
                    im_vec2_add(slider_active_p1, origin), ACTIVE_BACKGROUND);

  knob.draw_at(origin);
}
