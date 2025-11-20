#ifndef _BOUNDBOX_FRAME_H_
#define _BOUNDBOX_FRAME_H_

#include "runtime/geometry/core.h"
#include "nkengine/include/utils.h"

/**

  Since frames shape have selectable content insinde them, we can't define
  they're whole area as the selection trigger, as a result we need to compute an
  invisibile frame around them so they only get selected when we click within
  this area.

  +---+-----------------+---+
  | .-|-----------------|-. |
  +-;-+                 +-;-+
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  | ; |                 | ; |
  +-;-+-----------------+-;-+
  | '-|-----------------|-' |
  +---+-----------------+---+


 */

#define BOUNDBOX_FRAME_RECT_COUNT 4
typedef RectCoordinate BoundboxFrame[BOUNDBOX_FRAME_RECT_COUNT];

EXTERN_C_BEGIN

void boundbox_frame_update(BoundboxFrame, const vec2, const vec2, const float);

EXTERN_C_END

#endif
