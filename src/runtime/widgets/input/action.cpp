#include "action.hpp"
#include "nkengine/vendor/imgui/imgui_internal.h"
#include "resources/theme.emma.h"
#include "runtime/solutions/module/compounds/reward.h"
#include "runtime/solutions/module/core.h"
#include "runtime/widgets/input/segment.hpp"

#define REQUIRE_PTR(p)                                                         \
  do {                                                                         \
    if ((p) == nullptr) {                                                      \
      fprintf(stderr, "%s property not set for Action", #p);                   \
      return Status_ValidationFailed;                                          \
    }                                                                          \
  } while (0)

void Widget::Input::Action::Component::init(Gui *gui,
                                            Input::ActionParams *params) {
  this->gui = gui;
  this->params = params;

  map_action_to_params(this->params->handle);
  if (validate() == Status_Success) {
    init_inputs_from_params();
    layout();
  }
}

/**

   ▗▖  ▗▖ ▗▄▖ ▗▄▄▖
   ▐▛▚▞▜▌▐▌ ▐▌▐▌ ▐▌
   ▐▌  ▐▌▐▛▀▜▌▐▛▀▘
   ▐▌  ▐▌▐▌ ▐▌▐▌


   Since the action is kinda like a Set of Inputs, it would be quite cumbersombe
   to put all the parameters as argument. As a result we use a combination of
   API calls as well a validation to make sure everything is set properly before
   draw.

   This function is called right after the initialisation and before the layout.
   It acts as a more descriptive init().

   TODO: Currently we use many steps ton translate Solutions attributes into
   Inputs:

   Solution => Action => InputDescriptor => [ MAP ] => Input Component

   I feel like maybe the Action struct and Input Descriptor could be merged or
   something caue it feel very repetitive to map all the value.
   So streamline and simplify the whole thing.


 */
void Widget::Input::Action::Component::map_action_to_params(
    SolutionModuleAction *action) {

  set_label(action->action.label);
  set_role(&action->action.role);
  set_active(&action->action.active);

  // reward
  set_reward_probability(&action->reward.probability);
  set_reward_type(&action->reward.type);

  // amount
  const uint32_t MAX_REWARD_AMOUNT =
      compound_module_reward_get_max_amount(&action->reward);

  set_reward_amount_type(&action->reward.amount.type);
  set_reward_amount_fixed(&action->reward.amount.value, MAX_REWARD_AMOUNT);
  set_reward_amount_type(&action->reward.amount.type);
  set_reward_amount_range(&action->reward.amount.range.min,
                          &action->reward.amount.range.max, MAX_REWARD_AMOUNT);

  // frequency
  set_reward_frequency_quota(&action->reward.frequency.quota,
                             COMPOUND_MODULE_QUOTA_UNLIMITED);

  set_reward_frequency_interval(&action->reward.frequency.interval,
                                COMPOUND_MODULE_INTERVAL_LONGEST);

  set_reward_frequency_repeat(&action->reward.frequency.repeat,
                              COMPOUND_MODULE_REPEAT_ALWAYS);

  set_reward_frequency_forever(&action->reward.frequency.forever);
  set_reward_frequency_unit(&action->reward.frequency.unit);

  // time limit
  set_reward_time_limit_active(&action->reward.time_limit.active);

  const uint32_t MAX_TIME_LIMIT_AMOUNT =
      compound_module_reward_get_max_amount(&action->reward);

  set_reward_time_limit_amount(
      &action->reward.time_limit.amount,
      compound_module_reward_get_max_time_limit(&action->reward));

  set_reward_time_limit_unit(&action->reward.time_limit.unit);
}

/**
   Initialize it inputs from the components struct with the data from the
   param_list
 */
void Widget::Input::Action::Component::init_inputs_from_params() {

#define _(ParamType, VarName, CmpType, InputPath)                              \
  components.VarName.init(gui, &params_list.entries[ParamType].InputPath);

  ACTION_INPUT_ROUTE(_)

#undef _
}

/**
   Make sure all the components are set properly prior drawing.
 */
Widget::Input::Action::Component::Status
Widget::Input::Action::Component::validate() {

  // Core
  REQUIRE_PTR(label);
  REQUIRE_PTR(param(ParamType_Role)->segment.selected);
  REQUIRE_PTR(param(ParamType_Active)->checkbox.active);

  // Reward – type & probability
  REQUIRE_PTR(param(ParamType_Reward_Type)->combobox.selected);
  REQUIRE_PTR(param(ParamType_Reward_Probability)->amount.value);

  // Reward – amount
  REQUIRE_PTR(param(ParamType_Reward_Amount_Type)->combobox.selected);

  // Fixed OR Range is assumed to be handled by type,
  // but pointers must still be valid if used.
  if (param(ParamType_Reward_Amount_Value)->amount.value) {
    REQUIRE_PTR(param(ParamType_Reward_Amount_Value)->amount.value);
  } else {
    REQUIRE_PTR(param(ParamType_Reward_Amount_Min)->amount.value);
    REQUIRE_PTR(param(ParamType_Reward_Amount_Max)->amount.value);
  }

  // Reward – frequency
  REQUIRE_PTR(param(ParamType_Reward_FQ_Quota)->amount.value);
  REQUIRE_PTR(param(ParamType_Reward_FQ_Interval)->amount.value);
  REQUIRE_PTR(param(ParamType_Reward_FQ_Repeat)->amount.value);
  REQUIRE_PTR(param(ParamType_Reward_FQ_Forever)->checkbox.active);
  REQUIRE_PTR(param(ParamType_Reward_FQ_Unit)->combobox.selected);

  // Reward – time limit
  REQUIRE_PTR(param(ParamType_Reward_TL_Active)->checkbox.active);

  if (*param(ParamType_Reward_TL_Active)->checkbox.active) {
    REQUIRE_PTR(param(ParamType_Reward_TL_Amount)->amount.value);
    REQUIRE_PTR(param(ParamType_Reward_TL_Unit)->combobox.selected);
  }

  return Status_Success;
}

/*


   ▗▖    ▗▄▖▗▖  ▗▖▗▄▖ ▗▖ ▗▖▗▄▄▄▖
   ▐▌   ▐▌ ▐▌▝▚▞▘▐▌ ▐▌▐▌ ▐▌  █
   ▐▌   ▐▛▀▜▌ ▐▌ ▐▌ ▐▌▐▌ ▐▌  █
   ▐▙▄▄▖▐▌ ▐▌ ▐▌ ▝▚▄▞▘▝▚▄▞▘  █



 */

void Widget::Input::Action::Component::layout() {

  sizes.base_height = gui_scale(gui, 864);
  sizes.boundbox = ImVec2(gui_scale(gui, 368), sizes.base_height);
  sizes.closed_height =
      gui_scale(gui, emma_size(ThemeEmmaSize_Height_Input_Medium));

  {
    // set dynamic heights
    sizes.reward_amount_heights[CompoundModuleRewardAmountType_Undefined] =
        gui_scale(gui, 102);
    sizes.reward_amount_heights[CompoundModuleRewardAmountType_Fixed] =
        gui_scale(gui, 166);
    sizes.reward_amount_heights[CompoundModuleRewardAmountType_Range] =
        gui_scale(gui, 230);
  }

  update_height();

  sizes.content_width = sizes.boundbox.x - 2 * sizes.padding;

  sizes.padding = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Extra_Large_2));
  sizes.top_padding = gui_scale(gui, emma_size(ThemeEmmaSize_Space_Medium));

  sizes.reward_box_padding =
      gui_scale(gui, emma_size(ThemeEmmaSize_Space_Large));

  sizes.reward_total_padding = gui_scale(gui, 25);

  sizes.gap = emma_size(ThemeEmmaSize_Space_Extra_Large);
  sizes.radius = gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));

  sizes.header.height = gui_scale(gui, 32);

  sizes.reward_content_width = gui_scale(gui, 304);

  layout_header();
  layout_action();
  layout_reward();
  layout_amount();
  layout_frequency();
  layout_time_limit();
}
void Widget::Input::Action::Component::layout_header() {

  /*
    Checkbox Label      Chevron
        |      |           |
     [ x ]  Label         (V)
   */

  positions.header.checkbox.x = sizes.padding;
  positions.header.checkbox.y = sizes.top_padding + gui_scale(gui, 6);

  positions.header.chevron_start =
      ImVec2(sizes.boundbox.x - sizes.padding -
                 components.chevron_down.region->size[0],
             sizes.top_padding);

  positions.header.chevron_end =
      ImVec2(components.chevron_down.region->size[0],
             components.chevron_down.region->size[1]);

  positions.header.chevron_end =
      im_vec2_add(positions.header.chevron_start, positions.header.chevron_end);

  components.chevron_down.set_position(positions.header.chevron_start,
                                       GuiSpriteAnchor_TopLeft);
  components.chevron_up.set_position(positions.header.chevron_start,
                                     GuiSpriteAnchor_TopLeft);

  positions.header.underline_start =
      ImVec2(sizes.padding, sizes.top_padding + gui_scale(gui, 32));

  positions.header.underline_end =
      ImVec2(sizes.content_width + sizes.padding,
             positions.header.underline_start.y +
                 gui_scale(gui, emma_size(ThemeEmmaSize_Width_Border_Base)));
}
void Widget::Input::Action::Component::layout_action() {
  components.role.set_width(sizes.content_width);
}
void Widget::Input::Action::Component::layout_reward() {}

void Widget::Input::Action::Component::layout_amount() {
  positions.amount_end.x = sizes.content_width;
  positions.amount_end.y =
      sizes.reward_amount_heights[params->handle->reward.amount.type];

  components.reward_amount_type.set_width(sizes.reward_content_width);
  components.reward_amount_value.set_width(sizes.reward_content_width);
  components.reward_amount_min.set_width(sizes.reward_content_width);
  components.reward_amount_max.set_width(sizes.reward_content_width);
}
void Widget::Input::Action::Component::layout_frequency() {
  positions.frequency_end.x = sizes.content_width;
  positions.frequency_end.y = gui_scale(gui, 328 + sizes.reward_box_padding);

  components.reward_frequency_quota.set_width(sizes.reward_content_width);
  components.reward_frequency_interval.set_width(sizes.reward_content_width);
  components.reward_frequency_repeat.set_width(sizes.reward_content_width);
  components.reward_frequency_unit.set_width(sizes.reward_content_width);
}
void Widget::Input::Action::Component::layout_time_limit() {
  positions.time_limit_end.x = sizes.content_width;
  positions.time_limit_end.y = gui_scale(gui, 168 + sizes.reward_box_padding);

  components.reward_time_limit_amount.set_width(sizes.reward_content_width);
  components.reward_time_limit_unit.set_width(sizes.reward_content_width);
}

/**
   Update the total height by adding the dynamic height of the reward amout type
 */
void Widget::Input::Action::Component::update_height() {

  if (!open) {
    sizes.boundbox.y = sizes.closed_height;
    return;
  }

  sizes.boundbox.y =
      sizes.base_height +
      sizes.reward_amount_heights[params->handle->reward.amount.type] +
      sizes.padding;

  positions.amount_end.y =
      sizes.reward_amount_heights[params->handle->reward.amount.type];
}

/*

   ▗▄▄▄ ▗▄▄▖  ▗▄▖ ▗▖ ▗▖
   ▐▌  █▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌
   ▐▌  █▐▛▀▚▖▐▛▀▜▌▐▌ ▐▌
   ▐▙▄▄▀▐▌ ▐▌▐▌ ▐▌▐▙█▟▌


 */

bool Widget::Input::Action::Component::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();
  bool updated = false;

  // bg
  dl->AddRectFilled(origin, im_vec2_add(sizes.boundbox, origin),
                    colors.background, sizes.radius);

  ImGui::PushID(params->handle->action.label);

  draw_header(dl, &updated);

  if (open) {
    draw_gap();
    draw_action(dl, &updated);
    draw_gap();
    draw_reward(dl, &updated);
    draw_gap();
    draw_amount(dl, &updated);
    draw_gap();
    draw_frequency(dl, &updated);
    draw_gap();
    draw_time_limit(dl, &updated);
  }

  ImGui::PopID();

  ImGui::SetCursorScreenPos(origin);
  ImGui::ItemSize(sizes.boundbox);

  return updated;
}

void Widget::Input::Action::Component::draw_label(const char *label) {
  ImGui::SetWindowFontScale(1.1);
  ImGui::Text("%s", label);
  ImGui::SetWindowFontScale(1);
}

void Widget::Input::Action::Component::draw_frame(ImDrawList *dl,
                                                  const ImVec2 &start,
                                                  const ImVec2 &end) {
  dl->AddRect(start, end, colors.border, sizes.radius);
}

void Widget::Input::Action::Component::draw_header(ImDrawList *dl,
                                                   bool *updated) {

  ImVec2 origin = ImGui::GetCursorScreenPos();

  ImGui::SetCursorScreenPos(im_vec2_add(origin, positions.header.checkbox));
  if (components.active.draw())
    *updated = true;

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
      ImGui::IsMouseHoveringRect(
          im_vec2_add(positions.header.chevron_start, origin),
          im_vec2_add(positions.header.chevron_end, origin))) {
    open = !open;
    update_height();
  }

  if (open) {
    components.chevron_up.draw_at(origin);
    dl->AddRectFilled(im_vec2_add(origin, positions.header.underline_start),
                      im_vec2_add(origin, positions.header.underline_end),
                      colors.border);
  } else {
    components.chevron_down.draw_at(origin);
  }

  ImGui::SetCursorScreenPos(
      im_vec2_add(origin, ImVec2(0, sizes.header.height)));
}

void Widget::Input::Action::Component::draw_action(ImDrawList *dl,
                                                   bool *updated) {

  ImVec2 origin = ImGui::GetCursorScreenPos();
  ImGui::SetCursorPosX(sizes.padding);

  if (components.role.draw())
    *updated = true;
}

void Widget::Input::Action::Component::draw_reward(ImDrawList *dl,
                                                   bool *updated) {

  ImVec2 origin = ImGui::GetCursorScreenPos();

  ImGui::SetCursorPosX(sizes.padding);
  draw_label("Reward");
  draw_gap();

  ImGui::SetCursorPosX(sizes.padding);
  if (components.reward_type.draw())
    *updated = true;

  draw_gap();

  ImGui::SetCursorPosX(sizes.padding);
  if (components.reward_probability.draw())
    *updated = true;
}

void Widget::Input::Action::Component::draw_amount(ImDrawList *dl,
                                                   bool *updated) {

  ImVec2 origin;
  reward_section_begin(dl, "Amount", positions.amount_end, &origin);
  {
    draw_gap();

    reward_section_padding();
    if (components.reward_amount_type.draw()) {
      update_height();
      *updated = true;
    }

    if (CompoundModuleRewardAmountType_Fixed ==
        params->handle->reward.amount.type) {

      reward_section_padding();
      if (components.reward_amount_value.draw())
        *updated = true;

    } else if (CompoundModuleRewardAmountType_Range ==
               params->handle->reward.amount.type) {

      reward_section_padding();
      if (components.reward_amount_min.draw())
        *updated = true;

      draw_gap();

      reward_section_padding();
      if (components.reward_amount_max.draw())
        *updated = true;
    }
  }
  reward_section_end(origin, positions.amount_end);
}

void Widget::Input::Action::Component::draw_frequency(ImDrawList *dl,
                                                      bool *updated) {

  ImVec2 origin;
  reward_section_begin(dl, "Frequency", positions.frequency_end, &origin);
  {

    draw_gap();
    reward_section_padding();
    if (components.reward_frequency_quota.draw())
      *updated = true;

    draw_gap();
    reward_section_padding();
    if (components.reward_frequency_interval.draw())
      *updated = true;

    draw_gap();
    reward_section_padding();
    if (components.reward_frequency_repeat.draw())
      *updated = true;

    reward_section_padding();
    if (components.reward_frequency_forever.draw())
      *updated = true;

    draw_gap();
    reward_section_padding();
    if (components.reward_frequency_unit.draw())
      *updated = true;
  }
  reward_section_end(origin, positions.frequency_end);
}

void Widget::Input::Action::Component::draw_time_limit(ImDrawList *dl,
                                                       bool *updated) {

  ImVec2 origin;
  reward_section_begin(dl, "Time Limit", positions.time_limit_end, &origin);
  {
    draw_gap();
    reward_section_padding();
    if (components.reward_time_limit_amount.draw())
      *updated = true;

    draw_gap();
    reward_section_padding();
    if (components.reward_time_limit_unit.draw())
      *updated = true;
  }
  reward_section_end(origin, positions.time_limit_end);
}

void Widget::Input::Action::Component::reward_section_begin(
    ImDrawList *dl, const char *label, const ImVec2 &end, ImVec2 *dest_origin) {

  ImGui::PushID(label);

  ImGui::SetCursorPosX(sizes.padding);
  ImVec2 origin = ImGui::GetCursorScreenPos();

  if (dest_origin)
    *dest_origin = origin;

  draw_frame(dl, origin, im_vec2_add(end, origin));

  ImGui::SetCursorScreenPos(im_vec2_add(
      origin, ImVec2(sizes.reward_box_padding, sizes.reward_box_padding)));

  draw_label(label);
}

void Widget::Input::Action::Component::reward_section_end(const ImVec2 &origin,
                                                          const ImVec2 &end) {

  ImGui::PopID();
  ImGui::SetCursorScreenPos(origin);
  ImGui::ItemSize(ImVec2(sizes.reward_content_width, end.y));
}
