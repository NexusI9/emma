#ifndef _EMMA_SOLUTION_PERSONA_CONFIG_H_
#define _EMMA_SOLUTION_PERSONA_CONFIG_H_

#include "runtime/node/persona.h"
#include "runtime/solutions/persona/persona.h"

static const SolutionPersonaCompounds SOLUTION_PERSONAS[] = {

    [PersonaType_Operator] =
        {
            .social =
                {
                    .collaborative = 0.6f,
                    .collective = 0.7f,
                    .visibility = 0.2f,
                },
            .motivation =
                {
                    .epic_meaning = 0.20f,
                    .empowerment = 0.30f,
                    .social_influence = 0.65f,
                    .unpredictability = 0.15f,
                    .avoidance = 0.70f,
                    .scarcity = 0.20f,
                    .ownership = 0.25f,
                    .accomplishment = 0.35f,
                    .excitment = 0.30f,
                    .social_bounding = 0.75f,
                    .friction = 0.65f,
                    .reward = 0.40f,
                },
            .agency =
                {
                    .feedback_span = CompoundAgencyFeebackSpan_Short,
                    .learning_curve = CompoundAgencyLearningCurve_Gradual,
                    .risk_tolerance = 0.2f,
                },
            .density = {(float)1 / PersonaType_COUNT},
        },

    [PersonaType_Artisan] =
        {
            .social =
                {
                    .collaborative = 0.4f,
                    .collective = 0.2f,
                    .visibility = 0.3f,
                },
            .motivation =
                {
                    .epic_meaning = 0.65f,
                    .empowerment = 0.90f,
                    .social_influence = 0.20f,
                    .unpredictability = 0.40f,
                    .avoidance = 0.30f,
                    .scarcity = 0.35f,
                    .ownership = 0.40f,
                    .accomplishment = 0.90f,
                    .excitment = 0.45f,
                    .social_bounding = 0.40f,
                    .friction = 0.20f,
                    .reward = 0.30f,
                },
            .agency =
                {
                    .feedback_span = CompoundAgencyFeebackSpan_Long,
                    .learning_curve = CompoundAgencyLearningCurve_Instant,
                    .risk_tolerance = 0.45f,
                },
            .density = {(float)1 / PersonaType_COUNT},
        },

    [PersonaType_Giver] =
        {
            .social =
                {
                    .collaborative = 0.8f,
                    .collective = 0.9f,
                    .visibility = 0.4f,
                },
            .motivation =
                {
                    .epic_meaning = 0.70f,
                    .empowerment = 0.40f,
                    .social_influence = 0.85f,
                    .unpredictability = 0.20f,
                    .avoidance = 0.60f,
                    .scarcity = 0.10f,
                    .ownership = 0.15f,
                    .accomplishment = 0.60f,
                    .excitment = 0.40f,
                    .social_bounding = 0.95f,
                    .friction = 0.70f,
                    .reward = 0.15f,
                },
            .agency =
                {
                    .feedback_span = CompoundAgencyFeebackSpan_Long,
                    .learning_curve = CompoundAgencyLearningCurve_Gradual,
                    .risk_tolerance = 0.3f,
                },
            .density = {(float)1 / PersonaType_COUNT},
        },

    [PersonaType_Explorer] =
        {
            .social =
                {
                    .collaborative = 0.5f,
                    .collective = 0.3f,
                    .visibility = 0.6f,
                },
            .motivation =
                {
                    .epic_meaning = 0.75f,
                    .empowerment = 0.80f,
                    .social_influence = 0.45f,
                    .unpredictability = 0.95f,
                    .avoidance = 0.75f,
                    .scarcity = 0.40f,
                    .ownership = 0.30f,
                    .accomplishment = 0.80f,
                    .excitment = 0.90f,
                    .social_bounding = 0.45f,
                    .friction = 0.35f,
                    .reward = 0.45f,
                },
            .agency =
                {
                    .feedback_span = CompoundAgencyFeebackSpan_Short,
                    .learning_curve = CompoundAgencyLearningCurve_Instant,
                    .risk_tolerance = 0.85f,
                },
            .density = {(float)1 / PersonaType_COUNT},
        },

    [PersonaType_Striver] =
        {
            .social =
                {
                    .collaborative = 0.3f,
                    .collective = 0.4f,
                    .visibility = 0.8f,
                },
            .motivation =
                {
                    .epic_meaning = 0.45f,
                    .empowerment = 0.70f,
                    .social_influence = 0.90f,
                    .unpredictability = 0.25f,
                    .avoidance = 0.80f,
                    .scarcity = 0.85f,
                    .ownership = 0.90f,
                    .accomplishment = 0.85f,
                    .excitment = 0.70f,
                    .social_bounding = 0.65f,
                    .friction = 0.40f,
                    .reward = 0.95f,
                },
            .agency =
                {
                    .feedback_span = CompoundAgencyFeebackSpan_Short,
                    .learning_curve = CompoundAgencyLearningCurve_Gradual,
                    .risk_tolerance = 0.35f,
                },
            .density = {(float)1 / PersonaType_COUNT},
        },

    [PersonaType_Pioneer] =
        {
            .social =
                {
                    .collaborative = 0.6f,
                    .collective = 0.5f,
                    .visibility = 0.9f,
                },
            .motivation =
                {
                    .epic_meaning = 0.95f,
                    .empowerment = 0.95f,
                    .social_influence = 0.35f,
                    .unpredictability = 0.85f,
                    .avoidance = 0.25f,
                    .scarcity = 0.60f,
                    .ownership = 0.65f,
                    .accomplishment = 0.95f,
                    .excitment = 0.85f,
                    .social_bounding = 0.60f,
                    .friction = 0.25f,
                    .reward = 0.40f,
                },
            .agency =
                {
                    .feedback_span = CompoundAgencyFeebackSpan_Long,
                    .learning_curve = CompoundAgencyLearningCurve_Instant,
                    .risk_tolerance = 0.9f,
                },
            .density = {(float)1 / PersonaType_COUNT},
        },

};

#endif
