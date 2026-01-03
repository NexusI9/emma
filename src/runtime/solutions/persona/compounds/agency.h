#ifndef _EMMA_PERSONA_COMPOUND_AGENCY_H_
#define _EMMA_PERSONA_COMPOUND_AGENCY_H_

// Temporality Preference: Short vs Longer response time to see result
typedef enum {
  CompoundPersonaAgencyFeebackSpan_Short,
  CompoundPersonaAgencyFeebackSpan_Long,
  CompoundPersonaAgencyFeebackSpan_COUNT,
} CompoundPersonaAgencyFeebackSpan;

typedef enum {
  CompoundPersonaAgencyLearningCurve_Gradual,
  CompoundPersonaAgencyLearningCurve_Instant,
  CompoundPersonaAgencyLearningCurve_COUNT,
} CompoundPersonaAgencyLearningCurve;

typedef struct {

  CompoundPersonaAgencyFeebackSpan feedback_span;
  CompoundPersonaAgencyLearningCurve learning_curve;
  float risk_tolerance;

} CompoundPersonaAgency;

static const char *COMPOUND_PERSONA_FEEDBACK_SPAN_LABELS[] = {"Short", "Long"};
static const char *COMPOUND_PERSONA_LEARNING_CURVE_LABELS[] = {"Gradual", "Instant"};

/*
  Persona with a short feedback span preference and high risk tolerance will be
  thrilled by unpredictable events.
*/
static inline float compound_persona_agency_get_unpredictability(
    const CompoundPersonaAgency *compound) {

  static const float w_feedback = 0.7;
  static const float w_risk = 0.3;

  // clang-format off
  return   w_feedback * compound->feedback_span
         + w_risk * compound->risk_tolerance;
  // clang-format on
}

/*
 Persona with short span response and instant learning curve will be more
 receptive to avoidance and immediate pulse with feelings like “FOMO”
*/
static inline float
compound_persona_agency_get_avoidance(const CompoundPersonaAgency *compound) {

  static const float w_feedback = 0.7;
  static const float w_risk = 0.3;

  // clang-format off
  return   w_feedback * compound->feedback_span
         + w_risk * compound->risk_tolerance;
  // clang-format on
}

/*
  In this context we understand accomplishment as the successful outcome of an
  ongoing and sustainable process resulting from rigorous trials.

  In Accomplishment the “build” and “strive” aspects are important.
  Accomplishment is also linked to Intrinsic motivation, meaning the persona
  perform an action for its own sake and do not expect and immediate reward for
  it.

  As a result persona with more affinity toward longer response as well as
  gradual learning will be more keen to accomplishment settings like daily quest
  to reach a goal. As oppose to short span persona seeking instant learning who
  act based of short-term reward only.
*/
static inline float compound_persona_agency_get_accomplishment(
    const CompoundPersonaAgency *compound) {

  static const float w_feedback = 0.6;
  static const float w_learn = 0.4;

  // clang-format off
  return   w_feedback * compound->feedback_span
         + w_learn * (1 - compound->learning_curve);
  // clang-format on
}

/*
  Persona with instant learning curve and high risk tolerance will be more keen
  to empowerment for their motivation to explore and have full control up-front.
*/
static inline float
compound_persona_agency_get_empowerment(const CompoundPersonaAgency *compound) {

  static const float w_risk = 0.6;
  static const float w_learn = 0.4;

  // clang-format off
  return   w_risk * compound->risk_tolerance
         + w_learn * compound->learning_curve;
  // clang-format on
}

/*
  Persona with instant learning curve and short feedback span have a lower
  threshold to friction
*/
static inline float
compound_persona_agency_get_friction(const CompoundPersonaAgency *compound) {

  static const float w_feed = 0.6;
  static const float w_learn = 0.4;

  // clang-format off
  return   w_feed * (1 - compound->feedback_span)
         + w_learn * compound->learning_curve;
  // clang-format on
}

/*
  Persona with instant short feedback span, medium high risk tolerance and
  instant learning expectation are user with extrinsic purpose, meaning they are
  more interested in the outcome (and reward) rather than the experience itself.
*/
static inline float
compound_persona_agency_get_reward(const CompoundPersonaAgency *compound) {

  static const float w_feed = 0.6;
  static const float w_learn = 0.3;
  static const float w_risk = 0.1;

  // clang-format off
  return   w_feed * (1 - compound->feedback_span)
         + w_learn * compound->learning_curve
         + w_risk * compound->risk_tolerance;
  // clang-format on
}

#endif
