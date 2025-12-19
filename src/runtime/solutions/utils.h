#ifndef _EMMA_MODULE_COMPOUND_UTILS_H_
#define _EMMA_MODULE_COMPOUND_UTILS_H_

#include <inttypes.h>
#include <math.h>

static inline float norm(const int v, const int max) {
  return fminf((float)v / max, 1.0f);
}

static inline float inv_norm(const int v, const int max) {
  return 1 - (float)v / max;
}

static inline uint64_t range_avg(const uint64_t min, const uint64_t max) {
  return (min + max) / 2;
}

static inline uint64_t polarize(const float v) { return 2 * v - 1; }

#endif
