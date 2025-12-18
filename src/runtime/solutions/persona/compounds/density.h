#ifndef _EMMA_PERSONA_COMPOUND_DENSITY_H_
#define _EMMA_PERSONA_COMPOUND_DENSITY_H_

typedef struct {

  /* Value:
     The estimated proportion of target persona within a domain (department,
     unit, company...) between 0 and 100. (in percent basically)
   */
  float value;

} CompoundDensity;

static inline float
compound_density_get_value(const CompoundDensity *compound) {
  return compound->value;
}

#endif
