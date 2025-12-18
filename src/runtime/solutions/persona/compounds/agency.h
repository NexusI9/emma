#ifndef _EMMA_PERSONA_COMPOUND_AGENCY_H_
#define _EMMA_PERSONA_COMPOUND_AGENCY_H_

// Temporality Preference: Short vs Longer response time to see result
typedef enum {
  CompoundAgencyFeebackSpan_Short,
  CompoundAgencyFeebackSpan_Long,
} CompoundAgencyFeebackSpan;

typedef enum {
  CompoundAgencyLearningCurve_Gradual,
  CompoundAgencyLearningCurve_Instant,
} CompoundAgencyLearningCurve;

typedef struct {

  CompoundAgencyFeebackSpan feedback_span;
  CompoundAgencyLearningCurve learning_curve;
  float risk_tolerance;

} CompoundAgency;

/*
  Persona with a short feedback span preference and high risk tolerance will be
  thrilled by unpredictable events.
*/
static inline float
compound_agency_get_unpredictability(const CompoundAgency *compound) {

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
compound_agency_get_avoidance(const CompoundAgency *compound) {

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
static inline float
compound_agency_get_accomplishment(const CompoundAgency *compound) {

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
compound_agency_get_empowerment(const CompoundAgency *compound) {

  static const float w_risk = 0.6;
  static const float w_learn = 0.4;

  // clang-format off
  return   w_risk * compound->risk_tolerance
         + w_learn * compound->learning_curve;
  // clang-format on
}

#endif
