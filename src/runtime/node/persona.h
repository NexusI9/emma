#ifndef _EMMA_PERSONA_H_
#define _EMMA_PERSONA_H_

#include "nkengine/include/texture.h"

static const int PERSONA_COUNT = 6;

typedef enum {
  PersonaType_Operator,
  PersonaType_Artisan,
  PersonaType_Giver,
  PersonaType_Explorer,
  PersonaType_Striver,
  PersonaType_Pioneer,
} PersonaType;


const TextureAtlasRegion* persona_get_sprite(const PersonaType);

#endif
