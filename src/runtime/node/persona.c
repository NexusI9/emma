#include "persona.h"
#include "runtime/manager/ui_sprite.h"
#include <stdlib.h>

const TextureAtlasRegion *persona_get_sprite(const PersonaType type) {

  static const UISprite persona_sprite[] = {
      // clang-format off
      [ PersonaType_Artisan  ]  =  UISprite_Persona_Artisan,
      [ PersonaType_Giver    ]  =  UISprite_Persona_Giver,
      [ PersonaType_Operator ]  =  UISprite_Persona_Operator,
      [ PersonaType_Explorer ]  =  UISprite_Persona_Explorer,
      [ PersonaType_Pioneer  ]  =  UISprite_Persona_Pioneer,
      [ PersonaType_Striver  ]  =  UISprite_Persona_Striver,
      // clang-format on
  };

  return ui_sprite(persona_sprite[type]);
}

void persona_create_rand_coordinate(const vec2 p0, const vec2 p1,
                                    const float padding, const vec2 rad,
                                    const vec2 rot, vec2 dest_pos,
                                    float *dest_scale, float *dest_rot) {

  vec2 mid;
  vec2_avg(p0, p1, mid);

  float big_radius = glm_vec2_distance((float *)p0, mid);

  *dest_scale = rad[0] + ((float)rand() / RAND_MAX) * (rad[1] - rad[0]);

  *dest_rot = rot[0] + ((float)rand() / RAND_MAX) * (rot[1] - rot[0]);

  float allowed_radius = big_radius - padding - *dest_scale;
  if (allowed_radius < 0.0f)
    allowed_radius = 0.0f;

  float angle = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
  float r = sqrtf((float)rand() / RAND_MAX) * allowed_radius;

  // middle position
  dest_pos[0] = mid[0] + cosf(angle) * r;
  dest_pos[1] = mid[1] + sinf(angle) * r;

  // p0
  dest_pos[0] -= *dest_scale / 2;
  dest_pos[1] -= *dest_scale / 2;
}
