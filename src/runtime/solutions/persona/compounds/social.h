#ifndef _EMMA_PERSONA_COMPOUND_SOCIAL_H_
#define _EMMA_PERSONA_COMPOUND_SOCIAL_H_

typedef struct {

  /* Collaboartive:
     [-] Competitive
     [+] Cooperative
   */
  float collaborative;

  /* Collective:
     [-] Individual
     [+] Collective
   */
  float collective;

  /* Visbility:
     [-] Private
     [+] Public
   */
  float visibility;

} CompoundSocial;

/*
  Persona with a high competitiveness, collectiveness and public interest will
  lead to a high resonance with social influence, seeing others success will
  lead envy and motivate them to be like them and do better.
 */
static inline float
compound_persona_get_social_influence(const CompoundSocial *compound) {

  static const float w_collab = 0.5;
  static const float w_collect = 0.3;
  static const float w_vis = 0.2;

  // clang-format off
  return   w_collab * (1 - compound->collaborative)
         + w_collect * compound->collective
         + w_vis * compound->visibility;
  // clang-format on  
}


/*
  Persona with collaborative and public interests with be very keen to social bounding.
 */
static inline float
compound_persona_get_social_bounding(const CompoundSocial *compound) {

  static const float w_collab = 0.3;
  static const float w_collect = 0.5;
  static const float w_vis = 0.2;

  // clang-format off
  return   w_collab * compound->collaborative
         + w_collect * compound->collective
         + w_vis * compound->visibility;
  // clang-format on  
}


#endif
