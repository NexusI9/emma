#include "heatmap_select.hpp"
#include "resources/theme.emma.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/utils.hpp"
#include "utils/input.h"

Widget::Heatmap::Selector::Component::Component(const char *label, Gui *gui,
                                                ::Heatmap *heatmaps[Type_COUNT])
    : Widget(gui), Window(label), heatmaps{
                                      {gui, heatmaps[0]},
                                      {gui, heatmaps[1]},
                                      {gui, heatmaps[2]},
                                      {gui, heatmaps[3]},
                                  } {
  name_compose(list_name, "%slist", label);
  layout();
}

void Widget::Heatmap::Selector::Component::layout() {

  init_size =
      ImVec2(gui_scale(gui, context_width()), gui_scale(gui, context_height()));
  size = init_size;

  {
    list_position = ImVec2(gui_scale(gui, 1275), gui_scale(gui, 80));

    list_frame = UI::Frame();
    list_frame.background_color = emma_im_color(ThemeEmmaColor_Surface_Lower);
    list_frame.border_color =
        emma_im_color(ThemeEmmaColor_Border_Subtle_On_Dark);
    list_frame.padding =
        gui_scale_im_vec2(gui, ImVec2(emma_size(ThemeEmmaSize_Space_Medium),
                                      emma_size(ThemeEmmaSize_Space_Medium)));
    list_frame.border_radius =
        gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));
    list_frame.position = list_position;
    list_frame.size = UI::Frame::SIZE_AUTO;

    list_window_position = im_vec2_sub(list_position, list_frame.padding);
  }
}

void Widget::Heatmap::Selector::Component::listen_update() {

  if ((State_WheelMove & state) && !input_wheel_moving())
    state |= State_RequireUpdate;

  if (input_wheel_moving())
    state |= State_WheelMove;
  else
    state &= ~State_WheelMove;

  if (State_RequireUpdate & state) {
    state &= ~State_RequireUpdate;
    heatmaps[active_heatmap].compute_offline(gui->command_encoder);
  }
}

void Widget::Heatmap::Selector::Component::draw() {

  {
    UI::DockedWindow().Begin(list_name, list_window_position, size,
                             ImGuiWindowFlags_AlwaysAutoResize);
    if (!(State_Hidden & state))
      draw_list();
    UI::DockedWindow().End();
  }

  {
    // overlay part, need to set no inputs, so the heatmap doesn't obstruct the
    // canvas and can still register wheel movement
    UI::DockedWindow().Begin(label, ImVec2(0, 0), size,
                             ImGuiWindowFlags_NoInputs);

    if (!(State_Hidden & state))
      heatmaps[active_heatmap].draw();

    UI::DockedWindow().End();
  }
}

void Widget::Heatmap::Selector::Component::draw_list() {

  list_frame.Begin("##Heatmap list");

  ImGui::Text("Factors:");
  ImGui::Dummy(ImVec2(gui_scale(gui, 3.0f), gui_scale(gui, 3.0f)));
  for (int i = 0; i < Type_COUNT; i++) {
    if (ImGui::RadioButton(heatmaps[i].get_label(), active_heatmap == i)) {
      active_heatmap = (Type)i;
      state |= State_RequireUpdate;
    }
    ImGui::Dummy(ImVec2(gui_scale(gui, 3.0f), gui_scale(gui, 3.0f)));
  }

  list_frame.End();
}
