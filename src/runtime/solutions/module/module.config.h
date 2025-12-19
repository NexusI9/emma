#ifndef _EMMA_SOLUTION_MODULE_CONFIG_H_
#define _EMMA_SOLUTION_MODULE_CONFIG_H_

#include "runtime/manager/module.h"
#include "runtime/solutions/module/compounds/action.h"
#include "runtime/solutions/module/compounds/experience.h"
#include "runtime/solutions/module/compounds/reward.h"
#include "runtime/solutions/module/module.h"

static const SolutionModuleCompounds SOLUTION_MODULES[] = {

    [ModuleType_News] =
        {
            .experience =
                {
                    .impact = CompoundExperienceImpact_Routine,
                    .learnability = CompoundExperienceLearnability_Medium,
                },
            .semantic =
                {
                    .empowerment = 0,
                    .epic_meaning = 0,
                },
            .social =
                {
                    .collaborative = false,
                    .competitive = false,
                    .published = false,
                    .shareable = false,
                },
            .actions =
                {
                    .entries =
                        {
                            {
                                .action = {"See Article",
                                           CompoundActionRole_Primary},
                                .reward = {CompoundRewardType_Undefined},
                            },
                            {
                                .action = {"See All",
                                           CompoundActionRole_Secondary},
                                .reward = {CompoundRewardType_Undefined},
                            },

                        },

                    .count = 2,
                },
        },

    [ModuleType_Ranking] =
        {
            .experience =
                {
                    .impact = CompoundExperienceImpact_Important,
                    .learnability = CompoundExperienceLearnability_Easy,
                },
            .semantic =
                {
                    .empowerment = 3,
                    .epic_meaning = 2,
                },
            .social =
                {
                    .collaborative = true,
                    .competitive = true,
                    .published = true,
                    .shareable = true,
                },
            .actions =
                {
                    .entries =
                        {
                            {
                                .action = {"See Profile",
                                           CompoundActionRole_Tertiary},
                                .reward = {CompoundRewardType_Undefined},
                            },
                        },
                    .count = 1,
                },
        },

    [ModuleType_Rate] =
        {
            .experience =
                {
                    .impact = CompoundExperienceImpact_Routine,
                    .learnability = CompoundExperienceLearnability_Medium,
                },
            .semantic =
                {
                    .empowerment = 5,
                    .epic_meaning = 2,
                },
            .social =
                {
                    .collaborative = true,
                    .competitive = false,
                    .published = true,
                    .shareable = true,
                },
            .actions =
                {
                    .entries =
                        {
                            {
                                .action = {"Comment",
                                           CompoundActionRole_Primary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Coin,
                                        .amount =
                                            {
                                                .type =
                                                    CompoundRewardAmountType_Range,
                                                .range = {10, 20},
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_REWARD_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_REWARD_FREQUENCY_BASE,
                                    },
                            },
                            {
                                .action = {"Like",
                                           CompoundActionRole_Secondary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Point,
                                        .amount =
                                            {
                                                .type =
                                                    CompoundRewardAmountType_Fixed,
                                                .value = 5,
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_REWARD_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_REWARD_FREQUENCY_BASE,
                                    },
                            },
                            {
                                .action = {"Rate", CompoundActionRole_Tertiary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Point,
                                        .amount =
                                            {
                                                .type =
                                                    CompoundRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_REWARD_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_REWARD_FREQUENCY_BASE,
                                    },
                            },
                        },
                    .count = 3,
                },
        },

    [ModuleType_SpinningWheel] =
        {
            .experience =
                {
                    .impact = CompoundExperienceImpact_Trivial,
                    .learnability = CompoundExperienceLearnability_Easy,
                },
            .semantic =
                {
                    .empowerment = 2,
                    .epic_meaning = 1,
                },
            .social =
                {
                    .collaborative = false,
                    .competitive = false,
                    .published = false,
                    .shareable = true,
                },
            .actions =
                {
                    .entries =
                        {
                            {
                                .action = {"Spin the wheel",
                                           CompoundActionRole_Primary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Coin,
                                        .amount =
                                            {
                                                .type =
                                                    CompoundRewardAmountType_Fixed,
                                                .range = -10,
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_REWARD_PROBABILITY_ALWAYS,
                                        .frequency =
                                            {
                                                // 3 times every 2 day, forever
                                                .quota = 3,
                                                .interval = 2,
                                                .unit =
                                                    CompoundRewardTimeUnit_Day,
                                                .repeat =
                                                    COMPOUNT_REWARD_FREQUENCY_REPEAT_ALWAYS,
                                            },
                                    },
                            },
                            {
                                .action =
                                    {"Collect", CompoundActionRole_Secondary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Bundle,
                                        .amount =
                                            {
                                                .type = CompoundRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability = (float)1 / 200,
                                        .frequency =
                                            COMPOUND_REWARD_FREQUENCY_BASE,
                                    },
                            },
                            {
                                .action = {"Invite",
                                           CompoundActionRole_Tertiary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Coin,
                                        .amount =
                                            {
                                                .type = CompoundRewardAmountType_Fixed,
                                                .value = 150,
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_REWARD_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_REWARD_FREQUENCY_BASE,
                                    },
                            },
                        },
                    .count = 3,
                },
        },

    [ModuleType_SurveyCheckbox] =
        {
            .experience =
                {
                    .impact = CompoundExperienceImpact_Important,
                    .learnability = CompoundExperienceLearnability_Medium,
                },
            .semantic =
                {
                    .empowerment = 5,
                    .epic_meaning = 0,
                },
            .social =
                {
                    .collaborative = false,
                    .competitive = false,
                    .published = false,
                    .shareable = true,
                },
            .actions =
                {
                    .entries =
                        {
                            {
                                .action = {"Tick", CompoundActionRole_Primary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Coin,
                                        .amount =
                                            {
                                                .type = CompoundRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_REWARD_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_REWARD_FREQUENCY_BASE,
                                    },
                            },
                            {
                                .action = {"Share",
                                           CompoundActionRole_Tertiary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Point,
                                        .amount =
                                            {
                                                .type = CompoundRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_REWARD_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_REWARD_FREQUENCY_BASE,
                                    },
                            },
                        },
                    .count = 2,
                },
        },

    [ModuleType_SurveySlider] =
        {
            .experience =
                {
                    .impact = CompoundExperienceImpact_Important,
                    .learnability = CompoundExperienceLearnability_Medium,
                },
            .semantic =
                {
                    .empowerment = 5,
                    .epic_meaning = 0,
                },
            .social =
                {
                    .collaborative = false,
                    .competitive = false,
                    .published = false,
                    .shareable = true,
                },
            .actions =
                {
                    .entries =
                        {
                            {
                                .action = {"Tick", CompoundActionRole_Primary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Coin,
                                        .amount =
                                            {
                                                .type = CompoundRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_REWARD_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_REWARD_FREQUENCY_BASE,
                                    },
                            },
                            {
                                .action = {"Share",
                                           CompoundActionRole_Tertiary},
                                .reward =
                                    {
                                        .type = CompoundRewardType_Point,
                                        .amount =
                                            {
                                                .type = CompoundRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_REWARD_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_REWARD_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_REWARD_FREQUENCY_BASE,
                                    },
                            },
                        },
                    .count = 2,
                },
        },
};

#endif
