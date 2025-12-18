#ifndef _EMMA_MODULE_COMPOUND_REWARD_H_
#define _EMMA_MODULE_COMPOUND_REWARD_H_

#include "runtime/solutions/module/compounds/utils.h"
#include <inttypes.h>
#include <math.h>

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
    _(  Undefined,              0,              0.000   )\
    _(  Action,                10,              1.000   )\
    _(  Second,                60,              0.900   )\
    _(  Minute,                60,              0.750   )\
    _(  Hour,                  24,              0.500   )\
    _(  Day,                    7,              0.250   )\
    _(  Week,                   4,              0.100   )\
    _(  Month,                 12,              0.050   )\
    _(  Year,                   1,              0.005   )

// clang-format on

typedef enum {
#define _(Label, Amount, Weight) CompoundRewardType_##Label,
  REWARD_TYPES(_)
#undef _
} CompoundRewardType;

typedef enum {
#define _(Label, Amount, Weight) CompoundRewardCycleUnit_##Label,
  REWARD_CYCLES(_)
#undef _
} CompoundRewardCycleUnit;

typedef struct {
  uint64_t quantity;
  CompoundRewardCycleUnit unit;
} CompoundRewardCycle;

typedef enum {
  CompoundRewardAmountType_Undefined,
  CompoundRewardAmountType_Fixed,
  CompoundRewardAmountType_Range,
} CompoundRewardAmountType;

typedef struct {

  CompoundRewardAmountType type;

  union {
    uint64_t value;
    struct {
      uint64_t min, max;
      // TODO: add rules (random, level based...)
    } range;
  };

} CompoundRewardAmount;

typedef struct {
  // amount of reward per cycle
  uint64_t amount_per_cycle;
  CompoundRewardCycle cycle;
} CompoundRewardFrequency;

typedef struct {
  CompoundRewardType type;
  CompoundRewardAmount amount;
  CompoundRewardCycle time_limit;
  CompoundRewardFrequency frequency;
  float probability;
} CompoundReward;

// Presets

// 1N reward for 1 action, most common use case
static const CompoundRewardFrequency COMPOUND_REWARD_FREQUENCY_BASE = {
    1, {1, CompoundRewardCycleUnit_Action}};

static const float COMPOUND_REWARD_PROBABILITY_ALWAYS = 1.0f;

static const CompoundRewardCycle COMPOUND_REWARD_TIME_LIMIT_NONE = {0};

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
static inline bool compound_reward_null(const CompoundReward *set) {

  if (CompoundRewardType_Undefined == set->type)
    return true;

  if (CompoundRewardAmountType_Fixed == set->amount.type &&
      set->amount.value == 0)
    return true;

  if (CompoundRewardAmountType_Range == set->amount.type &&
      set->amount.range.min == 0 && set->amount.range.max == 0)
    return true;

  return false;
}

/*
  For each reward type we set a different amount of max amount.
 */
static inline uint64_t
compound_reward_get_max_amount(const CompoundReward *set) {

  static const uint64_t amounts[] = {
#define _(Label, Amount, Weight) [CompoundRewardType_##Label] = Amount,
      REWARD_TYPES(_)
#undef _
  };

  return amounts[set->amount.type];
}

/*
  Depending on their worth, we set different weight for each reward type (1 TV
  is more valuable that 10 coins)
 */
static inline float compound_reward_get_type_weight(const CompoundReward *set) {

  static const float weights[] = {
#define _(Label, Amount, Weight) [CompoundRewardType_##Label] = Weight,
      REWARD_TYPES(_)
#undef _
  };

  return weights[set->amount.type];
}

/*
  Each cycles unit has a different weight, if a cycle depends on the number of
  action it will have a higher weight that a cycle based on a year as it's
  easier for a user to satisfy the cycle (3 actions vs 1 year).
 */
static inline float
compound_reward_get_cycle_weight(const CompoundReward *set) {

  static const float weights[] = {
#define _(Label, Amount, Weight) [CompoundRewardCycleUnit_##Label] = Weight,
      REWARD_CYCLES(_)
#undef _
  };

  return weights[set->frequency.cycle.unit];
}

static inline float
compound_reward_cycle_unit_to_sec(const CompoundRewardCycle *cycle) {

  switch (cycle->unit) {

  case CompoundRewardCycleUnit_Undefined:
  case CompoundRewardCycleUnit_Action:
    return 0.0f;

  case CompoundRewardCycleUnit_Second:
    return cycle->quantity;

  case CompoundRewardCycleUnit_Minute:
    return cycle->quantity * 60.0f;

  case CompoundRewardCycleUnit_Hour:
    return cycle->quantity * 60.0f * 60.0f;

  case CompoundRewardCycleUnit_Day:
    return cycle->quantity * 24.0f * 60.0f * 60.0f;

  case CompoundRewardCycleUnit_Week:
    return cycle->quantity * 7.0f * 24.0f * 60.0f * 60.0f;

  case CompoundRewardCycleUnit_Month:
    return cycle->quantity * 30.0f * 24.0f * 60.0f * 60.0f;

  case CompoundRewardCycleUnit_Year:
    return cycle->quantity * 365.0f * 24.0f * 60.0f * 60.0f;
  }

  return 0.0f;
}

/*
  The reward frequency is the result of how many time in a given cycle we can
  get the reward.

 */
static inline float
compound_reward_compute_frequency(const CompoundReward *set) {

  if (CompoundRewardCycleUnit_Undefined == set->frequency.cycle.unit)
    return 0.0;

  float denom = 1.0f;

  // convert to seconds if unit is not Action based but time based
  if (CompoundRewardCycleUnit_Action != set->frequency.cycle.unit)
    denom = compound_reward_cycle_unit_to_sec(&set->frequency.cycle);

  return (float)set->frequency.amount_per_cycle /
         set->frequency.cycle.quantity * denom;
}

static inline float
compound_reward_compute_time_limit(const CompoundReward *set) {

  if (CompoundRewardCycleUnit_Undefined == set->frequency.cycle.unit)
    return 0.0;

  float factor = 1.0f;

  // convert to seconds if unit is not Action based but time based
  if (CompoundRewardCycleUnit_Action != set->frequency.cycle.unit)
    factor = compound_reward_cycle_unit_to_sec(&set->frequency.cycle);

  return set->time_limit.quantity * factor;
}

/*
  For each reward type we set a different amount of max amount.
 */
static inline uint64_t
compound_reward_get_max_time_limit(const CompoundReward *set) {

  static const uint64_t amounts[] = {
#define _(Label, Amount, Weight) [CompoundRewardCycleUnit_##Label] = Amount,
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
static inline float compound_reward_get_scarcity(const CompoundReward *set) {

  if (compound_reward_null(set))
    return 0.0;

  // Weights for scarcity components
  static const float w_amount = 0.35f;
  static const float w_frequency = 0.30f;
  static const float w_proba = 0.20f;
  static const float w_time = 0.15f;

  // Often invert the factor (fq, proba) cause high fq/proba // means low
  // scarcity

  const uint64_t max_amount = compound_reward_get_max_amount(set);

  // use average if range
  const uint64_t amount =
      (set->amount.type == CompoundRewardAmountType_Fixed)
          ? set->amount.value
          : range_avg(set->amount.range.min, set->amount.range.max);

  const float s_amount = 1.0f - ((float)amount / max_amount);

  const float f = compound_reward_compute_frequency(set);
  const float s_frequency = inv_norm(f, 1.0f);

  const float s_probability = 1.0f - set->probability;

  // time limit
  const float L = compound_reward_compute_time_limit(set);
  const float s_time_limit =
      inv_norm(L, compound_reward_get_max_time_limit(set));

  // Only add the type weight with the amount, cause 200 points < 1 TV (physical
  // gift)

  // clang-format off
  return   w_amount * s_amount * compound_reward_get_type_weight(set)
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
static inline float compound_reward_get_ownership(const CompoundReward *set) {

  if (compound_reward_null(set) || (CompoundRewardType_Badge != set->type &&
                                    CompoundRewardType_Bundle != set->type &&
                                    CompoundRewardType_Skin != set->type &&
                                    CompoundRewardType_Gift != set->type))
    return 0.0f;

  const uint64_t max_amount = compound_reward_get_max_amount(set);

  // use average if range
  const uint64_t amount =
      (set->amount.type == CompoundRewardAmountType_Fixed)
          ? set->amount.value
          : range_avg(set->amount.range.min, set->amount.range.max);

  return norm(amount, max_amount) * compound_reward_get_type_weight(set);
}

/*
  Mostly Based on the amount and type as well as time (expiration).
  Meaning a TV available for 1 day will generate more avoidance that 200 points
  for 2 weeks.
 */
static inline float compound_reward_get_avoidance(const CompoundReward *set) {

  if (compound_reward_null(set))
    return 0.0;

  static const float w_amount = 0.6f;
  static const float w_time = 0.4f;

  const uint64_t max_amount = compound_reward_get_max_amount(set);

  // use average if range
  const uint64_t amount =
      (set->amount.type == CompoundRewardAmountType_Fixed)
          ? set->amount.value
          : range_avg(set->amount.range.min, set->amount.range.max);

  // time limit
  const float L = compound_reward_compute_time_limit(set);
  const float time_limit = inv_norm(L, compound_reward_get_max_time_limit(set));

  // clang-format off
  return   w_amount * norm(amount, max_amount) * compound_reward_get_type_weight(set)
         + w_time * time_limit;
  // clang-format on
}

/*
  Based on the type and amount as well as low frequency and short limit
  time. If we give user only short time or few tries it will most definitely
  indulge thrill and stress to get the reward and make the experience more
  thrilling.
 */
static inline float compound_reward_get_excitement(const CompoundReward *set) {

  if (compound_reward_null(set))
    return 0.0;

  static const float w_amount = 0.4f;
  static const float w_time = 0.3f;
  static const float w_freq = 0.2f;

  const uint64_t max_amount = compound_reward_get_max_amount(set);

  // use average if range
  const uint64_t amount =
      (set->amount.type == CompoundRewardAmountType_Fixed)
          ? set->amount.value
          : range_avg(set->amount.range.min, set->amount.range.max);

  // fq
  const float f = compound_reward_compute_frequency(set);
  const float frequency = inv_norm(f, 1.0f);

  // time limit
  const float L = compound_reward_compute_time_limit(set);
  const float time_limit = inv_norm(L, compound_reward_get_max_time_limit(set));

  // clang-format off
  return   w_amount * norm(amount, max_amount) * compound_reward_get_type_weight(set)
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
compound_reward_get_accomplishment(const CompoundReward *set) {

  if (compound_reward_null(set))
    return 0.0;

  static const float w_amount = 0.6f;
  static const float w_freq = 0.4f;

  const uint64_t max_amount = compound_reward_get_max_amount(set);

  // use average if range
  const uint64_t amount =
      (set->amount.type == CompoundRewardAmountType_Fixed)
          ? set->amount.value
          : range_avg(set->amount.range.min, set->amount.range.max);

  // fq
  const float f = compound_reward_compute_frequency(set);
  const float frequency = inv_norm(f, 1.0f);

  // clang-format off
  return     w_amount * norm(amount, max_amount) * compound_reward_get_type_weight(set)
	   + w_freq * frequency;
  // clang-format on
}

/*
  Based on the probability factor and low frequency. Cause higher frequency
  means higher chance to get the reward. We should add some randomness in it
  instead of sheer probability, however it's not integrated yet in the system.
 */
static inline float
compound_reward_get_unpredictability(const CompoundReward *set) {

  if (compound_reward_null(set))
    return 0.0;

  static const float w_proba = 0.5f;
  static const float w_freq = 0.5f;

  const float probability = 1 - set->probability;

  const float f = compound_reward_compute_frequency(set);
  const float frequency = inv_norm(f, 1.0f);

  // clang-format off
  return   w_proba * probability
	 + w_freq * frequency;
  // clang-format on
}

static inline float compound_reward_get_reward(const CompoundReward *set) {

  if (compound_reward_null(set))
    return 0.0;

  // use average if range
  const uint64_t amount =
      (set->amount.type == CompoundRewardAmountType_Fixed)
          ? set->amount.value
          : range_avg(set->amount.range.min, set->amount.range.max);

  return norm(amount, compound_reward_get_max_amount(set)) *
         compound_reward_get_type_weight(set);
}

#endif
