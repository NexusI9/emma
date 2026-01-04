#ifndef _EMMA_PERSONA_H_
#define _EMMA_PERSONA_H_

#include "nkengine/include/texture.h"

static const uint8_t PERSONA_PADDING = 100;
static const uint8_t PERSONA_BASE_SIZE = 100;

static const float PERSONA_MIN_RADIUS = 0.7f;
static const float PERSONA_MAX_RADIUS = 1.0f;
static const float PERSONA_MIN_ANGLE = 0.2f;
static const float PERSONA_MAX_ANGLE = 0.6f;

typedef enum {
  PersonaType_Operator,
  PersonaType_Artisan,
  PersonaType_Giver,
  PersonaType_Explorer,
  PersonaType_Striver,
  PersonaType_Pioneer,
  PersonaType_COUNT,
} PersonaType;

static const struct {
  const char *label;
  const char *description;
} PERSONA_INTRO[PersonaType_COUNT] = {
    [PersonaType_Operator] =
        {
            "Operator",
            "See work mainly as a means to an end. Prefer stability, avoid "
            "standing out, and invest less in changing their future. "
            "Team-oriented and friendly, but can become disengaged.",
        },
    [PersonaType_Artisan] =
        {
            "Artisan",
            "Driven by mastery and craft, valuing autonomy and deep expertise. "
            "Want autonomy and recognition for expertise but care less about "
            "status or workplace camaraderie.",
        },
    [PersonaType_Giver] =
        {
            "Giver",
            "Find meaning in helping others and building trust through "
            "relationships. Least driven by money. Empathetic, "
            "relationship-focused, cautious, and good planners. Can be "
            "selfless but sometimes impractical.",
        },
    [PersonaType_Explorer] =
        {
            "Explorer",
            "Value freedom, variety, and experiences. Live in the present, "
            "embrace flexibility, and are willing to change careers. Energetic "
            "and adaptable but can lack direction.",
        },
    [PersonaType_Striver] =
        {
            "Striver",
            "Ambitious, success-oriented, and motivated by status and "
            "compensation. Risk-averse planners who follow conventional paths. "
            "Disciplined but may become overly competitive.",
        },
    [PersonaType_Pioneer] =
        {
            "Pioneer",
            "Driven by a vision to change the world. Risk-tolerant, "
            "future-focused, and deeply identified with their work. Inspiring "
            "change-makers but may be uncompromising or domineering.",
        },
};

const TextureAtlasRegion *persona_get_sprite(const PersonaType);

void persona_create_rand_coordinate(const vec2, const vec2, const float,
                                    const vec2, const vec2, vec2, float *,
                                    float *);

#endif
