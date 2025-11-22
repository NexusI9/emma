#ifndef _EMMA_GEOMETRY_VECTOR_H_
#define _EMMA_GEOMETRY_VECTOR_H_

#include <cglm/cglm.h>

static inline void vec2_avg_2(const vec2 p0, const vec2 p1, vec2 dest) {
  glm_vec2_add((float *)p0, (float *)p1, dest);
  glm_vec2_scale(dest, 0.5f, dest);
}

#endif
