#include "slider.hpp"
#include "nkengine/vendor/imgui/imgui_internal.h"

void Widget::Slider::Component::layout() {

  sizes.slider = ImVec2(gui_scale(gui, 324), gui_scale(gui, 4));
  sizes.dot = gui_scale(gui, emma_size(ThemeEmmaSize_Icon_Medium));
  sizes.row_gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));
  sizes.amount_gap = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Large));

  const int BASE_SLIDER_Y =
      positions.label.y + ImGui::GetFontSize() + sizes.row_gap;
  const int BAR_Y = BASE_SLIDER_Y + gui_scale(gui, 14);
  const int KNOB_Y = BASE_SLIDER_Y + gui_scale(gui, 3);

  {
    positions.slider_start.x = 0.0f;
    positions.slider_start.y = BAR_Y;

    positions.slider_end.x = sizes.slider.x;
    positions.slider_end.y = BAR_Y + sizes.slider.y;
  }

  positions.slider_active_start = positions.slider_start;
  positions.slider_active_end = positions.slider_end;

  sizes.clickbox.x = sizes.slider.x;
  sizes.clickbox.y = sizes.dot;

  components.knob.set_position(
      ImVec2(positions.slider_active_end.x, (float)KNOB_Y),
      GuiSpriteAnchor_TopLeft);

  positions.amount =
      ImVec2(positions.slider_active_end.x + sizes.amount_gap, KNOB_Y);

  if (params->value)
    update_value(*params->value);

  sizes.boundbox =
      ImVec2(sizes.slider.x, components.knob.get_end().y - positions.label.y);
}

void Widget::Slider::Component::update_value(const float value) {

  positions.slider_active_end.x = sizes.slider.x * value;
  components.knob.set_position(
      ImVec2(positions.slider_active_end.x, components.knob.get_start().y),
      value == 0.0f   ? GuiSpriteAnchor_TopLeft
      : value == 1.0f ? GuiSpriteAnchor_TopRight
                      : GuiSpriteAnchor_TopMiddle);

  switch (format) {

  case Format_Float:
    name_compose(str_value, "%.2f", value * params->multiplier);
    break;

  case Format_Integer:
    name_compose(str_value, "%d", (int)(value * params->multiplier));
    break;
  }
}

void Widget::Slider::Component::mouse_pos_to_value(const ImVec2 &origin) {

  // normalise mouse position
  float mouse_x = ImGui::GetIO().MousePos.x;
  float start_x = origin.x;
  float end_x = origin.x + sizes.slider.x;

  t = (mouse_x - start_x) / (end_x - start_x);
  t = ImClamp(t, 0.0f, 1.0f);

  // map t to value
  float v = params->min + t * (params->max - params->min);

  if (format == Format_Integer) {
    v = roundf(v);
    v = ImClamp((int)v, (int)params->min, (int)params->max);
  } else if (params->step > 0.0f) {
    v = roundf(v / params->step) * params->step;
  }

  if (v != *params->value)
    *params->value = v;
}

float Widget::Slider::Component::update_knob_position_from_value() {
  float value = (*params->value - params->min) / (params->max - params->min);
  return ImClamp(value, 0.0f, 1.0f);
}

bool Widget::Slider::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();
  bool updated = false;

  ImGui::ItemSize(sizes.boundbox);

  dl->AddText(im_vec2_add(positions.label, origin), colors.label,
              params->label);

  ImGui::SetWindowFontScale(1.3);
  dl->AddText(im_vec2_add(positions.amount, origin), colors.label, str_value);
  ImGui::SetWindowFontScale(1);

  const ImVec2 slider_bar_p0 = im_vec2_add(positions.slider_start, origin);

  // Interaction
  if (ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
      ImGui::IsMouseHoveringRect(slider_bar_p0,
                                 im_vec2_add(slider_bar_p0, sizes.clickbox))) {
    mouse_pos_to_value(origin);
    value_t = update_knob_position_from_value();
    update_value(value_t);
    updated = true;
  }

  dl->AddRectFilled(slider_bar_p0, im_vec2_add(positions.slider_end, origin),
                    colors.bg_default);

  dl->AddRectFilled(im_vec2_add(positions.slider_active_start, origin),
                    im_vec2_add(positions.slider_active_end, origin),
                    colors.bg_active);

  components.knob.draw_at(origin);

  return updated;
}
