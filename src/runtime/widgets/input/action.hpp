#ifndef _WIDGET_INPUT_ACTION_H_
#define _WIDGET_INPUT_ACTION_H_

#include "nkengine/include/gui.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/solutions/module/compounds/action.h"
#include "runtime/solutions/module/compounds/reward.h"
#include "runtime/solutions/module/core.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/input/amount.hpp"
#include "runtime/widgets/input/checkbox.hpp"
#include "runtime/widgets/input/combobox.hpp"
#include "runtime/widgets/input/input.hpp"
#include "runtime/widgets/input/segment.hpp"

namespace Widget {

namespace Input {

namespace Action {

// clang-format off
//    Parameter Type                     Variable Name            Input Component Object    Param Path 
#define ACTION_INPUT_ROUTE(_)                                                                                \
  _( ParamType_Active,               active,                      Checkbox::Component,      checkbox      )  \
  _( ParamType_Role,                 role,                        Segment::Component,       segment       )  \
											                     \
  _( ParamType_Reward_Type,          reward_type,                 Combobox::Component,      combobox      )  \
  _( ParamType_Reward_Probability,   reward_probability,          Amount::Component,        amount        )  \
											                     \
  _( ParamType_Reward_Amount_Type,   reward_amount_type,          Segment::Component,       segment       )  \
  _( ParamType_Reward_Amount_Value,  reward_amount_value,         Amount::Component,        amount        )  \
  _( ParamType_Reward_Amount_Min,    reward_amount_min,           Amount::Component,        amount        )  \
  _( ParamType_Reward_Amount_Max,    reward_amount_max,           Amount::Component,        amount        )  \
											                     \
  _( ParamType_Reward_FQ_Quota,      reward_frequency_quota,      Amount::Component,        amount        )  \
  _( ParamType_Reward_FQ_Interval,   reward_frequency_interval,   Amount::Component,        amount        )  \
  _( ParamType_Reward_FQ_Repeat,     reward_frequency_repeat,     Amount::Component,        amount        )  \
  _( ParamType_Reward_FQ_Forever,    reward_frequency_forever,    Checkbox::Component,      checkbox      )  \
  _( ParamType_Reward_FQ_Unit,       reward_frequency_unit,       Combobox::Component,      combobox      )  \
											                     \
  _( ParamType_Reward_TL_Active,     reward_time_limit_active,    Checkbox::Component,      checkbox      )  \
  _( ParamType_Reward_TL_Amount,     reward_time_limit_amount,    Amount::Component,        amount        )  \
  _( ParamType_Reward_TL_Unit,       reward_time_limit_unit,      Combobox::Component,      combobox      )
// clang-format on

class Component : public Widget {

public:
  Component()
      : Widget(nullptr),
        components{.chevron = {
                       texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                       ui_sprite(UISprite_Icon_Chevron_Down_Medium_White)}} {

    // set initial attributes
  }

  typedef enum {
    State_Opened,
    State_Closed,
  } State;

  typedef enum {
    Status_Success,
    Status_ValidationFailed,
  } Status;

  void init(Gui *, Input::ActionParams *);
  void map_action_to_params(SolutionModuleAction *);
  void init_inputs_from_params();
  Status validate();
  void layout();
  void draw();

  void set_label(const char *label) { this->label = label; }
  void set_role(CompoundModuleActionRole *role) {
    param(ParamType_Role)->segment.selected = (int *)role;
  }

  void set_active(bool *active) {
    param(ParamType_Active)->checkbox.active = active;
  }

  void set_reward_type(CompoundModuleRewardType *type) {
    param(ParamType_Reward_Type)->combobox.selected = (int *)type;
  }

  void set_reward_probability(float *probability) {
    param(ParamType_Reward_Probability)->amount.value = probability;
  };

  void set_reward_amount_type(CompoundModuleRewardAmountType *type) {
    param(ParamType_Reward_Amount_Type)->combobox.selected = (int *)type;
  }

  void set_reward_amount_fixed(uint32_t *value, uint32_t max) {
    param(ParamType_Reward_Amount_Value)->amount.value = (float *)value;
    param(ParamType_Reward_Amount_Value)->amount.max = max;
  }

  void set_reward_amount_range(uint32_t *min, uint32_t *max,
                               uint32_t absolute_max) {

    param(ParamType_Reward_Amount_Min)->amount.value = (float *)min;
    param(ParamType_Reward_Amount_Min)->amount.max = absolute_max;

    param(ParamType_Reward_Amount_Max)->amount.value = (float *)max;
    param(ParamType_Reward_Amount_Max)->amount.max = absolute_max;
  }

  void set_reward_frequency_quota(uint32_t *value, uint32_t max) {
    param(ParamType_Reward_FQ_Quota)->amount.value = (float *)value;
    param(ParamType_Reward_FQ_Quota)->amount.max = max;
  }

  void set_reward_frequency_interval(uint32_t *value, uint32_t max) {
    param(ParamType_Reward_FQ_Interval)->amount.value = (float *)value;
    param(ParamType_Reward_FQ_Interval)->amount.max = max;
  }

  void set_reward_frequency_repeat(uint32_t *value, uint32_t max) {
    param(ParamType_Reward_FQ_Repeat)->amount.value = (float *)value;
    param(ParamType_Reward_FQ_Repeat)->amount.max = max;
  }

  void set_reward_frequency_forever(bool *value) {
    param(ParamType_Reward_FQ_Forever)->checkbox.active = value;
  }

  void set_reward_frequency_unit(CompoundModuleRewardTimeUnit *unit) {
    param(ParamType_Reward_FQ_Unit)->combobox.selected = (int *)unit;
  }

  void set_reward_time_limit_active(bool *value) {
    param(ParamType_Reward_TL_Active)->checkbox.active = value;
  }

  void set_reward_time_limit_amount(uint32_t *value, uint32_t max) {
    param(ParamType_Reward_TL_Amount)->amount.value = (float *)value;
    param(ParamType_Reward_TL_Amount)->amount.max = max;
  }

  void set_reward_time_limit_unit(CompoundModuleRewardTimeUnit *unit) {
    param(ParamType_Reward_TL_Unit)->combobox.selected = (int *)unit;
  }

private:
  State state;
  const char *label;
  Input::ActionParams *params;

  typedef enum {
#define _(Type, VarName, CmpType, InputType) Type,
    ACTION_INPUT_ROUTE(_)
#undef _
        ParamType_COUNT,

  } ParamType;

  // Sections
  void layout_header();
  void layout_action();
  void layout_reward();
  void layout_amount();
  void layout_frequency();
  void layout_time_limit();

  void draw_header(ImDrawList *);
  void draw_action(ImDrawList *);
  void draw_reward(ImDrawList *);
  void draw_amount(ImDrawList *);
  void draw_frequency(ImDrawList *);
  void draw_time_limit(ImDrawList *);

  // utils
  void draw_gap() { ImGui::Dummy(ImVec2(sizes.boundbox.x, sizes.gap)); }
  void draw_label(const char *);
  void draw_frame(ImDrawList *, const ImVec2 &, const ImVec2 &);
  Input::Params *param(const ParamType type) {
    return &params_list.entries[type];
  }
  void reward_section_begin(ImDrawList *dl, const char *, const ImVec2 &,
                            ImVec2 *);
  void reward_section_end(const ImVec2 &, const ImVec2 &);
  void reward_section_padding() {
    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2(sizes.reward_total_padding, 0));
    ImGui::SameLine();
  }

  // positions
  struct {
    ImVec2 time_limit_switch;

    struct {
      ImVec2 checkbox;
      ImVec2 chevron;
      ImVec2 underline_start;
      ImVec2 underline_end;
    } header;

    ImVec2 reward_content;
    ImVec2 amount_end;
    ImVec2 frequency_end;
    ImVec2 time_limit_end;

  } positions;

  // sizes
  struct {
    ImVec2 boundbox;
    ImVec2 closed_boundbox;
    int content_width;
    int padding;
    int top_padding;
    int reward_box_padding;
    int reward_total_padding;
    int gap;
    int radius;
    int reward_content_width;

    struct {
      int height;
    } header;

  } sizes;

  // colors
  struct {
    const ImColor label = im_color(emma_color(ThemeEmmaColor_Text_On_Dark));
    const ImColor background =
        im_color(emma_color(ThemeEmmaColor_Surface_Lowest));
    const ImColor border =
        im_color(emma_color(ThemeEmmaColor_Border_Subtlest_On_Dark));
  } colors;

  // components (sprite, inputs, buttons...)
  struct {
    ::Component::Sprite chevron;

#define _(Type, VarName, CmpType, InputType) CmpType VarName;
    ACTION_INPUT_ROUTE(_)
#undef _
  } components;

  // others
  /*
     Since our components are based on <Input>Params* and work by reference, we
     need to cache each actions into input params.
     Although slightly cumbersome, this yet ensure a more concise control on the
     different parameters (min/ max, steps...).
     Initially the inputs takes params:

        1. for consistency purpose, in the sense that it felt more cumbersome to
        have a struct with the parameters and the actual component with a
        flatened copy of it. For synchronisation purpose it felt more meaningul
        to use the same source or truth as reference for performance and storage
        stake.

        2. for templating purpose since we automatically layout the inputs based
        on a discrimator array. As a result it's easier to check the input type
        and directly feed the parameters reference in the component on the fly.

     The challenge here is that we cannot use a 1 level linear input drawing
     based on a list like for the editor profile cause the design is segmented
     in numerous, not necessarily systematized sections.

     The Input List below can be seen as a cached descriptor list.
    */

  Input::List params_list = {
      .count = ParamType_COUNT,
      .entries =
          {
              // === Action ===

              [ParamType_Active] =
                  {
                      Input::Type_Checkbox,
                      .checkbox = {"Active"},
                  },
              [ParamType_Role] =
                  {
                      Input::Type_Segment,
                      .segment =
                          {
                              "Role",
                              .items = COMPOUND_MODULE_ACTION_ROLE_LABELS,
                              .count = CompoundModuleActionRole_COUNT,
                          },
                  },

              // === Reward ===

              [ParamType_Reward_Type] =
                  {
                      Input::Type_Combobox,
                      .segment =
                          {
                              "Type",
                              .items = COMPOUND_MODULE_REWARD_TYPE_LABELS,
                              .count = CompoundModuleRewardType_COUNT,
                          },
                  },
              [ParamType_Reward_Probability] =
                  {
                      Input::Type_Amount,
                      .amount =
                          {
                              "Probability (1/N)",
                              .min = 0,
                              .step = 1,
                          },
                  },

              // === Amount ===

              [ParamType_Reward_Amount_Type] =
                  {
                      Input::Type_Segment,
                      .segment =
                          {
                              "Type",
                              .items = COMPOUND_MODULE_REWARD_AMOUNT_LABELS,
                              .count = CompoundModuleRewardAmountType_COUNT,
                          },
                  },
              [ParamType_Reward_Amount_Value] =
                  {
                      Input::Type_Amount,
                      .amount =
                          {
                              "Value",
                              .min = 0,
                              .step = 1,
                          },
                  },

              // === Frequency ===

              [ParamType_Reward_FQ_Quota] =
                  {
                      Input::Type_Amount,
                      .amount =
                          {
                              "Quota",
                              .min = 0,
                              .step = 1,
                          },
                  },
              [ParamType_Reward_FQ_Interval] =
                  {
                      Input::Type_Amount,
                      .amount =
                          {
                              "Interval",
                              .min = 0,
                              .step = 1,
                          },
                  },
              [ParamType_Reward_FQ_Repeat] =
                  {
                      Input::Type_Amount,
                      .amount =
                          {
                              "Repeat",
                              .min = 0,
                              .step = 1,
                          },
                  },
              [ParamType_Reward_FQ_Forever] =
                  {
                      Input::Type_Checkbox,
                      .checkbox = {"Forever"},
                  },
              [ParamType_Reward_FQ_Unit] =
                  {
                      Input::Type_Combobox,
                      .combobox =
                          {
                              "Unit",
                              .items = COMPOUND_MODULE_REWARD_TIME_LABELS,
                              .count = CompoundModuleRewardTimeUnit_COUNT,
                          },
                  },

              // === Time Limit ===

              [ParamType_Reward_TL_Active] =
                  {
                      Input::Type_Checkbox,
                      .checkbox = {"Unit"},
                  },
              [ParamType_Reward_TL_Amount] =
                  {
                      Input::Type_Amount,
                      .amount =
                          {
                              "Amount",
                              .min = 0,
                              .step = 1,
                          },
                  },
              [ParamType_Reward_TL_Unit] =
                  {
                      Input::Type_Combobox,
                      .combobox =
                          {
                              "Unit",
                              .items = COMPOUND_MODULE_REWARD_TIME_LABELS,
                              .count = CompoundModuleRewardTimeUnit_COUNT,
                          },
                  },
          },
  };
};

} // namespace Action
} // namespace Input

} // namespace Widget

#endif
