#include "persona.h"

const TextureAtlasRegion *persona_get_sprite(const PersonaType type) {

  static const TextureAtlasRegion *persona_sprite[] = {};

  return persona_sprite[type];
}
