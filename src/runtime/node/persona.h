#ifndef _EMMA_PERSONA_H_
#define _EMMA_PERSONA_H_

#include "nkengine/include/texture.h"

static const uint8_t PERSONA_COUNT = 6;
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
} PersonaType;

const TextureAtlasRegion *persona_get_sprite(const PersonaType);

void persona_create_rand_coordinate(const vec2, const vec2, const float,
                                    const vec2, const vec2, vec2, float *,
                                    float *);

#endif
