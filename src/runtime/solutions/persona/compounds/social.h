#ifndef _EMMA_PERSONA_COMPOUND_SOCIAL_H_
#define _EMMA_PERSONA_COMPOUND_SOCIAL_H_

typedef struct {

  /* Collaboartive:
     [-] Competitive
     [+] Cooperative
   */
  float collaborative;

  /* Visbility:
     [-] Private
     [+] Public
   */
  float visibility;

} CompoundPersonaSocial;

/*
  Persona with a high competitiveness, collectiveness and public interest will
  lead to a high resonance with social influence, seeing others success will
  lead envy and motivate them to be like them and do better.
 */
static inline float compound_persona_social_get_influence(
    const CompoundPersonaSocial *compound) {

  static const float w_collab = 0.5;
  static const float w_vis = 0.2;

  // clang-format off
  return   w_collab * (1 - compound->collaborative)
         + w_vis * compound->visibility;
  // clang-format on  
}


/*
  Persona with collaborative and public interests with be very keen to social bounding.
 */
static inline float
compound_persona_social_get_bounding(const CompoundPersonaSocial *compound) {

  static const float w_collab = 0.3;
  static const float w_vis = 0.2;

  // clang-format off
  return   w_collab * compound->collaborative
         + w_vis * compound->visibility;
  // clang-format on  
}


#endif
