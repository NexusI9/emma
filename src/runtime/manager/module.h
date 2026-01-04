#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include "nkengine/include/texture.h"

typedef enum {
  ModuleType_News,
  ModuleType_Rate,
  ModuleType_SpinningWheel,
  ModuleType_Ranking,
  ModuleType_SurveyCheckbox,
  ModuleType_SurveySlider,
  ModuleType_COUNT,
} ModuleType;

static const struct {
  const char *label;
  const char *description;
} MODULE_INTRO[ModuleType_COUNT] = {
    [ModuleType_News] =
        {
            "News",
            "Delivers timely information and updates to keep users informed "
            "and context-aware, supporting transparency and lightweight "
            "engagement without requiring deep interaction.",
        },

    [ModuleType_Rate] =
        {
            "Rate",
            "Enables users to evaluate content, services, or outcomes through "
            "simple scoring mechanisms, providing structured feedback signals "
            "that support quality assessment and continuous improvement.",
        },

    [ModuleType_SpinningWheel] =
        {
            "Spinning Wheel",
            "Introduces chance-based outcomes to create moments of surprise "
            "and anticipation, encouraging engagement through randomness, "
            "rewards, and playful interaction loops.",
        },

    [ModuleType_Ranking] =
        {
            "Ranking",
            "Displays comparative performance or progress across users or "
            "entities, highlighting relative standing to stimulate "
            "competition, benchmarking, and long-term motivation.",
        },

    [ModuleType_SurveyCheckbox] =
        {
            "Survey Checkbox",
            "Collects discrete, multi-choice user input for preference capture "
            "and qualitative insights, supporting quick decision-making and "
            "categorical data aggregation.",
        },

    [ModuleType_SurveySlider] =
        {
            "Survey Slider",
            "Captures nuanced, continuous user input along a defined range, "
            "enabling fine-grained feedback, intensity measurement, and trend "
            "analysis.",
        },

};

EXTERN_C_BEGIN

const TextureAtlasRegion *get_module(const ModuleType);

EXTERN_C_END

#endif
