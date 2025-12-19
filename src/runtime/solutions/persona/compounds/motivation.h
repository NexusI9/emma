#ifndef _EMMA_PERSONA_COMPOUND_MOTIVATION_H_
#define _EMMA_PERSONA_COMPOUND_MOTIVATION_H_

/*

   === Core Drive ===

   Epic Meaning:
    [-] Pragmatic with fewer creativity
    [+] Visionary and high predisposition to imagination and narration

   Empowerment:
    [-] Require guidance
    [+] Strong autonomy and decision-making

   Social Influence:
    [-] High self-esteem, slight narcissist tendencies
    [+] Concerned about peers opinions and social trends

   Unpredictability:
    [-] Like stable and linear outcome
    [+] Player or Gambler mindset

   Avoidance:
    [-] Spiritually-centered and stable state of mind
    [+] Easily manipulated by external factor

   Scarcity:
    [-] Low interest in ownership and purchase-induced stress
    [+] High interest in collecting rare items or dominant consumer spirit

   Ownership:
    [-] Interested in human or spiritual connection an development
    [+] Materialistic interests

   Accomplishment:
    [-] Less involved in exploration
    [+] Curious, looking for self-improvement

   === Heatmap ===

   Excitment:
    [-] Introverted and moderate mindset
    [+] Extraverted and highly expressive personalities

   Social Bounding:
    [-] Individualist
    [+] Collectivist

   Friction:
    [-] Tech savvy user with more problem-solving capabilities
    [+] Casual user who can easily be lost

   Reward:
    [-] Intrinsic interests
    [+] Extrinsic interests
 */

#define MOTIVATIONS(_)                                                         \
  _(epic_meaning)                                                              \
  _(empowerment)                                                               \
  _(social_influence)                                                          \
  _(unpredictability)                                                          \
  _(avoidance)                                                                 \
  _(scarcity)                                                                  \
  _(ownership)                                                                 \
  _(accomplishment)                                                            \
  _(excitment)                                                                 \
  _(social_bounding)                                                           \
  _(friction)                                                                  \
  _(reward)

typedef struct {
#define _(Label) float Label;
  MOTIVATIONS(_)
#undef _
} CompoundPersonaMotivation;

#define _(Label)                                                               \
  static inline float compound_persona_motivation_get_##Label(                 \
      const CompoundPersonaMotivation *compound) {                             \
    return compound->Label;                                                    \
  }
MOTIVATIONS(_)
#undef _

#endif
