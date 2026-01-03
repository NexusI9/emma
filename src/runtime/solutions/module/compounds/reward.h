#ifndef _EMMA_MODULE_COMPOUND_REWARD_H_
#define _EMMA_MODULE_COMPOUND_REWARD_H_

#include "runtime/solutions/utils.h"
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

// clang-format off
/*
  Reward types:
  .-----------.-----------------------------------------------------------------------------.
  | Point     |  Most common reward currency, also known as 'XP', helps to level up         |
  |-----------+-----------------------------------------------------------------------------|   
  | Coin      |  Common yet valuable reward as collecting it allow gift purchase            |
  |-----------+-----------------------------------------------------------------------------|
  | Badge     |  Collectible items with social value                                        |
  |-----------+-----------------------------------------------------------------------------|
  | Privilege |  Status that allow access to dedicated mechanics (veterans, loyal user)     |
  |-----------+-----------------------------------------------------------------------------|
  | Boost     |  Often temporary items, giving considerable advantages                      |
  |-----------+-----------------------------------------------------------------------------| 
  | Bundle    |  Lootboxes, Mystery Package bundling multiple rewards                       |
  |-----------+-----------------------------------------------------------------------------| 
  | Skin      |  Themes, or customized avatar, backgrounds, patterns                        |
  |-----------+-----------------------------------------------------------------------------| 
  | Gift      |  Physical goods, coupons, can be used in real life                          |
  '-----------'-----------------------------------------------------------------------------'
 */

//       Label     |     Max Amount      |     Weight
#define REWARD_TYPES(_)		                           \
    _(  Undefined,               0,              0.000     )\
    _(  Point,                5000,              0.100     )\
    _(  Coin,                 1000,              0.200     )\
    _(  Boost,                   8,              0.400     )\
    _(  Badge,                  10,              0.450     )\
    _(  Privilege,               6,              0.500     )\
    _(  Bundle,                  1,              0.750     )\
    _(  Skin,                    1,              0.850     )\
    _(  Gift,                    1,              1.000     )


/*
  For the cycles max amount we use a clamp based value, as instance if the second is more that 60,
  it means that it should use a minute based time-limit.
  Such constrain will be enforced in front-end/GUI side making sure the values are clamped.
 */
//       Label     |     Max Amount      |    Weight
#define REWARD_CYCLES(_)                                \
    _(  Undefined,              1,              0.000   )\
    _(  Action,                 5,              1.000   )\
    _(  Second,                60,              0.900   )\
    _(  Minute,                60,              0.750   )\
    _(  Hour,                  24,              0.500   )\
    _(  Day,                    7,              0.250   )\
    _(  Week,                   4,              0.100   )\
    _(  Month,                 12,              0.050   )\
    _(  Year,                   1,              0.005   )

// clang-format on

typedef enum {
#define _(Label, Amount, Weight) CompoundModuleRewardType_##Label,
  REWARD_TYPES(_)
#undef _
} CompoundModuleRewardType;

typedef enum {
#define _(Label, Amount, Weight) CompoundModuleRewardTimeUnit_##Label,
  REWARD_CYCLES(_)
#undef _
} CompoundModuleRewardTimeUnit;

typedef enum {
  CompoundModuleRewardAmountType_Undefined,
  CompoundModuleRewardAmountType_Fixed,
  CompoundModuleRewardAmountType_Range,
  CompoundModuleRewardAmountType_COUNT,
} CompoundModuleRewardAmountType;

static const char *COMPOUND_MODULE_REWARD_AMOUNT_LABELS[] = {"Undefined",
                                                             "Fixed", "Range"};

static const char *COMPOUND_MODULE_REWARD_TYPE_LABELS[] = {
#define _(Label, Amount, Weight) #Label,
    REWARD_TYPES(_)
#undef _
};

static const char *COMPOUND_MODULE_REWARD_TIME_LABELS[] = {
#define _(Label, Amount, Weight) #Label,
    REWARD_CYCLES(_)
#undef _
};

typedef struct {

  CompoundModuleRewardAmountType type;

  union {
    uint32_t value;
    struct {
      uint32_t min, max;
      // TODO: add rules (random, level based...)
    } range;
  };

} CompoundModuleRewardAmount;

typedef struct {
  uint32_t quota;    // how many times available per cycle (e.g. 3)
  uint32_t interval; // cycle length (e.g. 2)
  uint32_t repeat;   // how many cycles occur (0 or UINT64_MAX = infinite)
  CompoundModuleRewardTimeUnit unit;
} CompoundModuleRewardFrequency;

typedef struct {
  uint64_t amount;
  CompoundModuleRewardTimeUnit unit;
} CompoundModuleRewardTimeLimit;

typedef struct {
  CompoundModuleRewardType type;
  CompoundModuleRewardAmount amount;
  CompoundModuleRewardTimeLimit time_limit;
  CompoundModuleRewardFrequency frequency;
  float probability;
} CompoundModuleReward;

// Presets

static const uint32_t COMPOUND_MODULE_QUOTA_UNLIMITED = INT32_MAX;
static const uint32_t COMPOUND_MODULE_REPEAT_ALWAYS = INT32_MAX;
static const float COMPOUND_MODULE_PROBABILITY_ALWAYS = 1.0f;

// 1N reward for 1 action, most common use case
static const CompoundModuleRewardFrequency COMPOUND_MODULE_FREQUENCY_BASE = {
    .repeat = COMPOUND_MODULE_REPEAT_ALWAYS,
    .interval = 1,
    .quota = 1,
    .unit = CompoundModuleRewardTimeUnit_Action,
};

static const CompoundModuleRewardTimeLimit COMPOUND_MODULE_TIME_LIMIT_NONE = {
    0};

static const float COMPOUNT_MODULE_AMOUNT_SATURATION = 0.2f;

/*

   ▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖    ▗▄▄▖
   ▐▌ ▐▌  █    █  ▐▌   ▐▌
   ▐▌ ▐▌  █    █  ▐▌    ▝▀▚▖
   ▝▚▄▞▘  █  ▗▄█▄▖▐▙▄▄▖▗▄▄▞▘

 */

/*
   Return true if reward configuration actually has an amount > 0 or a valid
   type (non-undefined)
 */
static inline bool
compound_module_reward_null(const CompoundModuleReward *set) {

  if (CompoundModuleRewardType_Undefined == set->type)
    return true;

  if (CompoundModuleRewardAmountType_Fixed == set->amount.type &&
      set->amount.value == 0)
    return true;

  if (CompoundModuleRewardAmountType_Range == set->amount.type &&
      set->amount.range.min == 0 && set->amount.range.max == 0)
    return true;

  return false;
}

/*
  For each reward type we set a different amount of max amount.
 */
static inline uint64_t
compound_module_reward_get_max_amount(const CompoundModuleReward *set) {

  static const uint64_t amounts[] = {
#define _(Label, Amount, Weight) [CompoundModuleRewardType_##Label] = Amount,
      REWARD_TYPES(_)
#undef _
  };

  return amounts[set->amount.type];
}

/*
  Depending on their worth, we set different weight for each reward type (1 TV
  is more valuable that 10 coins)

  However to align with observe distribution and real case scenario we use power
  to reduce distance between weights.

 */
static inline float
compound_module_reward_get_type_weight(const CompoundModuleReward *set) {

  static const float SATURATION = 0.225;

  static const float weights[] = {
#define _(Label, Amount, Weight) [CompoundModuleRewardType_##Label] = Weight,
      REWARD_TYPES(_)
#undef _
  };

  return powf(weights[set->amount.type], SATURATION);
}

/*
  Each cycles unit has a different weight, if a cycle depends on the number of
  action it will have a higher weight that a cycle based on a year as it's
  easier for a user to satisfy the cycle (3 actions vs 1 year).
 */
static inline float
compound_module_reward_get_cycle_weight(const CompoundModuleReward *set) {

  static const float weights[] = {
#define _(Label, Amount, Weight)                                               \
  [CompoundModuleRewardTimeUnit_##Label] = Weight,
      REWARD_CYCLES(_)
#undef _
  };

  return weights[set->frequency.unit];
}

static inline float compound_module_reward_cycle_unit_to_sec(
    const float value, const CompoundModuleRewardTimeUnit unit) {

  switch (unit) {

  case CompoundModuleRewardTimeUnit_Undefined:
    return 1.0f;

  case CompoundModuleRewardTimeUnit_Action:
    // in average the Click Per Second (CPS) or Touch Per Second is around 4 to
    // 7, we will use 5 as a reference value.
    return 5.0f;

  case CompoundModuleRewardTimeUnit_Second:
    return value;

  case CompoundModuleRewardTimeUnit_Minute:
    return value * 60.0f;

  case CompoundModuleRewardTimeUnit_Hour:
    return value * 60.0f * 60.0f;

  case CompoundModuleRewardTimeUnit_Day:
    return value * 24.0f * 60.0f * 60.0f;

  case CompoundModuleRewardTimeUnit_Week:
    return value * 7.0f * 24.0f * 60.0f * 60.0f;

  case CompoundModuleRewardTimeUnit_Month:
    return value * 30.0f * 24.0f * 60.0f * 60.0f;

  case CompoundModuleRewardTimeUnit_Year:
    return value * 365.0f * 24.0f * 60.0f * 60.0f;
  }

  return 1.0f;
}

static inline float
compound_module_norm_amount(const CompoundModuleReward *set) {

  const uint64_t amount =
      (set->amount.type == CompoundModuleRewardAmountType_Fixed)
          ? set->amount.value
          : range_avg(set->amount.range.min, set->amount.range.max);

  return log_norm(amount, compound_module_reward_get_max_amount(set),
                  COMPOUNT_MODULE_AMOUNT_SATURATION);
}

/*
  The reward frequency is the result of how many time in a given cycle we can
  get the reward.
 */
static inline float
compound_module_reward_compute_frequency(const CompoundModuleReward *set) {

  if (CompoundModuleRewardTimeUnit_Undefined == set->frequency.unit)
    return 0.0;

  const float repeat_factor =
      (set->frequency.repeat == 0 ||
       set->frequency.repeat == COMPOUND_MODULE_REPEAT_ALWAYS)
          ? 1.0f
          : logf(set->frequency.repeat);

  return (float)(repeat_factor * set->frequency.quota) /
         compound_module_reward_cycle_unit_to_sec(set->frequency.interval,
                                                  set->frequency.unit);
}

/*
  For each reward type we set a different amount of max amount.
 */
static inline uint64_t
compound_module_reward_get_max_time_limit(const CompoundModuleReward *set) {

  static const uint64_t amounts[] = {
#define _(Label, Amount, Weight)                                               \
  [CompoundModuleRewardTimeUnit_##Label] = Amount,
      REWARD_CYCLES(_)
#undef _
  };

  return amounts[set->time_limit.unit];
}

/*

    ▗▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖ ▗▄▄▄▖▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖ ▗▄▄▖
   ▐▌ ▐▌ █    █  ▐▌ ▐▌  █  ▐▌ ▐▌▐▌ ▐▌  █  ▐▌   ▐▌
   ▐▛▀▜▌ █    █  ▐▛▀▚▖  █  ▐▛▀▚▖▐▌ ▐▌  █  ▐▛▀▀▘ ▝▀▚▖
   ▐▌ ▐▌ █    █  ▐▌ ▐▌▗▄█▄▖▐▙▄▞▘▝▚▄▞▘  █  ▐▙▄▄▖▗▄▄▞▘


 */

/*
  The scarcity is a mix of:

  - the amount of the reward we get
  - mixed with its probability of winning it
  - how many time a cycle we get it
  - finally, the limit of time we can get it
 */
static inline float
compound_module_reward_get_scarcity(const CompoundModuleReward *set) {

  if (compound_module_reward_null(set))
    return 0.0;

  // Weights for scarcity components
  static const float w_amount = 0.35f;
  static const float w_frequency = 0.30f;
  static const float w_proba = 0.20f;
  static const float w_time = 0.15f;

  // Often invert the factor (fq, proba) cause high fq/proba // means low
  // scarcity

  const uint64_t max_amount = compound_module_reward_get_max_amount(set);

  // use average if range
  const float amount = compound_module_norm_amount(set);
  const float s_amount = 1.0f - amount;

  const float f = compound_module_reward_compute_frequency(set);
  const float s_frequency = inv_norm(f, 1.0f);

  const float s_probability = 1.0f - set->probability;

  // time limit
  const float L = compound_module_reward_cycle_unit_to_sec(
      set->time_limit.amount, set->time_limit.unit);
  const float s_time_limit =
      inv_norm(L, compound_module_reward_get_max_time_limit(set));

  // Only add the type weight with the amount, cause 200 points < 1 TV (physical
  // gift)

  // clang-format off
  return   w_amount * s_amount * compound_module_reward_get_type_weight(set)
         + w_frequency * s_frequency
         + w_proba * s_probability 
         + w_time * s_time_limit;
  // clang-format on
}

/*
  Based on what kind of reward we get (badge, skin) and its amount.
  It’s 0 if the reward is below Badges as Coins or Points don’t produce any
  ownership.
 */
static inline float
compound_module_reward_get_ownership(const CompoundModuleReward *set) {

  if (compound_module_reward_null(set) ||
      (CompoundModuleRewardType_Badge != set->type &&
       CompoundModuleRewardType_Bundle != set->type &&
       CompoundModuleRewardType_Skin != set->type &&
       CompoundModuleRewardType_Gift != set->type))
    return 0.0f;

  const float amount = compound_module_norm_amount(set);

  return amount * compound_module_reward_get_type_weight(set);
}

/*
  Mostly Based on the amount and type as well as time (expiration).
  Meaning a TV available for 1 day will generate more avoidance that 200 points
  for 2 weeks.
 */
static inline float
compound_module_reward_get_avoidance(const CompoundModuleReward *set) {

  if (compound_module_reward_null(set))
    return 0.0;

  static const float w_amount = 0.6f;
  static const float w_time = 0.4f;

  const uint64_t max_amount = compound_module_reward_get_max_amount(set);

  // use average if range
  const float amount = compound_module_norm_amount(set);

  // time limit
  const float L = compound_module_reward_cycle_unit_to_sec(
      set->time_limit.amount, set->time_limit.unit);
  const float time_limit =
      inv_norm(L, compound_module_reward_get_max_time_limit(set));

  // clang-format off
  return   w_amount * amount * compound_module_reward_get_type_weight(set)
         + w_time * time_limit;
  // clang-format on
}

/*
  Based on the type and amount as well as low frequency and short limit
  time. If we give user only short time or few tries it will most definitely
  indulge thrill and stress to get the reward and make the experience more
  thrilling.
 */
static inline float
compound_module_reward_get_excitement(const CompoundModuleReward *set) {

  if (compound_module_reward_null(set))
    return 0.0;

  static const float w_amount = 0.4f;
  static const float w_time = 0.3f;
  static const float w_freq = 0.2f;

  const uint64_t max_amount = compound_module_reward_get_max_amount(set);

  // use average if range
  const float amount = compound_module_norm_amount(set);
  // fq
  const float f = compound_module_reward_compute_frequency(set);
  const float frequency = inv_norm(f, 1.0f);

  // time limit
  const float L = compound_module_reward_cycle_unit_to_sec(
      set->time_limit.amount, set->time_limit.unit);
  const float time_limit =
      inv_norm(L, compound_module_reward_get_max_time_limit(set));

  // clang-format off
  return   w_amount * amount * compound_module_reward_get_type_weight(set)
	 + w_freq * frequency
         + w_time * time_limit;
  // clang-format on
}

/*
   Based on the type and amount or reward coupled with a low frequency. If the
   reward feel deserved (i.e. not delivered all the time), it will generate more
   accomplishment.
 */
static inline float
compound_module_reward_get_accomplishment(const CompoundModuleReward *set) {

  if (compound_module_reward_null(set))
    return 0.0;

  static const float w_amount = 0.6f;
  static const float w_freq = 0.4f;

  // use average if range
  const float amount = compound_module_norm_amount(set);
  // fq
  const float f = compound_module_reward_compute_frequency(set);
  const float frequency = inv_norm(f, 1.0f);

  // clang-format off
  return  w_amount * amount * compound_module_reward_get_type_weight(set)
	+ w_freq * frequency;
  // clang-format on
}

/*
  Based on the probability factor and low frequency. Cause higher frequency
  means higher chance to get the reward. We should add some randomness in it
  instead of sheer probability, however it's not integrated yet in the system.
 */
static inline float
compound_module_reward_get_unpredictability(const CompoundModuleReward *set) {

  if (compound_module_reward_null(set))
    return 0.0;

  static const float w_proba = 0.5f;
  static const float w_freq = 0.5f;

  const float probability = 1 - set->probability;

  const float f = compound_module_reward_compute_frequency(set);
  const float frequency = inv_norm(f, 1.0f);

  // clang-format off
  return   w_proba * probability
	 + w_freq * frequency;
  // clang-format on
}

/*
  To get the reward factor we need to use a normalized amount, as a result each
  reward type has their respective max amount according to theoretical ceiling:
  coins can go up to 10000, however scarcer reward like gift or badges should
  remain low to preserve their value.

  Nonetheless, using theoretical ceiling is not enough as this contradict
  Observed Distribution. Meaning although theoretically we want the max coins to
  be 10000, 99% of the time the amount of coin will be between 1 and 40.

  Using a linear normalization completely breaks the reward output by making the
  number extremely unisgnificant if using commons values (1-40).

  As a result we need to use a Logarithmic Normalization in order to still make
  the reward factor significant and valuable although the reward is relatively
  small compared to the max amount.
 */
static inline float
compound_module_reward_get_reward(const CompoundModuleReward *set) {

  if (compound_module_reward_null(set))
    return 0.0;

  // use average if range
  const float amount = compound_module_norm_amount(set);

  return amount * compound_module_reward_get_type_weight(set);
}

#endif
