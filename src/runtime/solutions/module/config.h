#ifndef _EMMA_SOLUTION_MODULE_CONFIG_H_
#define _EMMA_SOLUTION_MODULE_CONFIG_H_

#include "runtime/manager/module.h"
#include "runtime/solutions/module/compounds/action.h"
#include "runtime/solutions/module/compounds/experience.h"
#include "runtime/solutions/module/compounds/reward.h"
#include "runtime/solutions/module/core.h"

static const SolutionModule SOLUTION_MODULES[] = {

    [ModuleType_News] =
        {
            .experience =
                {
                    .impact = CompoundModuleExperienceImpact_Routine,
                    .learnability = CompoundModuleExperienceLearnability_Medium,
                },
            .semantic =
                {
                    .empowerment = 0,
                    .epic_meaning = 3,
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
                                .action = {"See Article",
                                           CompoundModuleActionRole_Primary},
                                .reward = {CompoundModuleRewardType_Undefined},
                            },
                            {
                                .action = {"See All",
                                           CompoundModuleActionRole_Secondary},
                                .reward = {CompoundModuleRewardType_Undefined},
                            },

                        },

                    .count = 2,
                },
        },

    [ModuleType_Ranking] =
        {
            .experience =
                {
                    .impact = CompoundModuleExperienceImpact_Important,
                    .learnability = CompoundModuleExperienceLearnability_Easy,
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
                                           CompoundModuleActionRole_Tertiary},
                                .reward = {CompoundModuleRewardType_Undefined},
                            },
                        },
                    .count = 1,
                },
        },

    [ModuleType_Rate] =
        {
            .experience =
                {
                    .impact = CompoundModuleExperienceImpact_Routine,
                    .learnability = CompoundModuleExperienceLearnability_Medium,
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
                                           CompoundModuleActionRole_Primary},
                                .reward =
                                    {
                                        .type = CompoundModuleRewardType_Coin,
                                        .amount =
                                            {
                                                .type =
                                                    CompoundModuleRewardAmountType_Range,
                                                .range = {10, 20},
                                            },
                                        .time_limit =
                                            COMPOUND_MODULE_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_MODULE_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_MODULE_FREQUENCY_BASE,
                                    },
                            },
                            {
                                .action = {"Like",
                                           CompoundModuleActionRole_Secondary},
                                .reward =
                                    {
                                        .type = CompoundModuleRewardType_Point,
                                        .amount =
                                            {
                                                .type =
                                                    CompoundModuleRewardAmountType_Fixed,
                                                .value = 5,
                                            },
                                        .time_limit =
                                            COMPOUND_MODULE_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_MODULE_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_MODULE_FREQUENCY_BASE,
                                    },
                            },
                            {
                                .action = {"Rate",
                                           CompoundModuleActionRole_Tertiary},
                                .reward =
                                    {
                                        .type = CompoundModuleRewardType_Point,
                                        .amount =
                                            {
                                                .type =
                                                    CompoundModuleRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_MODULE_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_MODULE_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_MODULE_FREQUENCY_BASE,
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
                    .impact = CompoundModuleExperienceImpact_Trivial,
                    .learnability = CompoundModuleExperienceLearnability_Easy,
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
                                           CompoundModuleActionRole_Primary},
                                .reward =
                                    {
                                        .type = CompoundModuleRewardType_Coin,
                                        .amount =
                                            {
                                                .type = CompoundModuleRewardAmountType_Fixed,
                                                .value = 1000,
                                            },
                                        .time_limit =
                                            COMPOUND_MODULE_TIME_LIMIT_NONE,
                                        .probability = (float)1 / 200,
                                        .frequency =
                                            {
                                                // 3 times every 2 day, forever
                                                .quota = 3,
                                                .interval = 1,
                                                .unit = CompoundModuleRewardTimeUnit_Day,
                                                .repeat =
                                                    COMPOUND_MODULE_REPEAT_ALWAYS,
                                            },
                                    },
                            },
                            {
                                .action = {"Invite",
                                           CompoundModuleActionRole_Secondary},
                                .reward =
                                    {
                                        .type = CompoundModuleRewardType_Coin,
                                        .amount =
                                            {
                                                .type = CompoundModuleRewardAmountType_Fixed,
                                                .value = 150,
                                            },
                                        .time_limit =
                                            COMPOUND_MODULE_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_MODULE_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_MODULE_FREQUENCY_BASE,
                                    },
                            },
                        },
                    .count = 2,
                },
        },

    [ModuleType_SurveyCheckbox] =
        {
            .experience =
                {
                    .impact = CompoundModuleExperienceImpact_Important,
                    .learnability = CompoundModuleExperienceLearnability_Medium,
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
                                .action = {"Tick",
                                           CompoundModuleActionRole_Primary},
                                .reward =
                                    {
                                        .type = CompoundModuleRewardType_Coin,
                                        .amount =
                                            {
                                                .type = CompoundModuleRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_MODULE_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_MODULE_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_MODULE_FREQUENCY_BASE,
                                    },
                            },
                            {
                                .action = {"Share",
                                           CompoundModuleActionRole_Tertiary},
                                .reward =
                                    {
                                        .type = CompoundModuleRewardType_Point,
                                        .amount =
                                            {
                                                .type = CompoundModuleRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_MODULE_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_MODULE_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_MODULE_FREQUENCY_BASE,
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
                    .impact = CompoundModuleExperienceImpact_Important,
                    .learnability = CompoundModuleExperienceLearnability_Medium,
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
                                .action = {"Tick",
                                           CompoundModuleActionRole_Primary},
                                .reward =
                                    {
                                        .type = CompoundModuleRewardType_Coin,
                                        .amount =
                                            {
                                                .type = CompoundModuleRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_MODULE_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_MODULE_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_MODULE_FREQUENCY_BASE,
                                    },
                            },
                            {
                                .action = {"Share",
                                           CompoundModuleActionRole_Tertiary},
                                .reward =
                                    {
                                        .type = CompoundModuleRewardType_Point,
                                        .amount =
                                            {
                                                .type = CompoundModuleRewardAmountType_Fixed,
                                                .value = 1,
                                            },
                                        .time_limit =
                                            COMPOUND_MODULE_TIME_LIMIT_NONE,
                                        .probability =
                                            COMPOUND_MODULE_PROBABILITY_ALWAYS,
                                        .frequency =
                                            COMPOUND_MODULE_FREQUENCY_BASE,
                                    },
                            },
                        },
                    .count = 2,
                },
        },
};

#endif
