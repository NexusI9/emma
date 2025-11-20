#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "nkengine/include/gui.h"
#include "nkengine/include/renderer.h"
#include "runtime/allocator.h"
#include "runtime/node/frame.h"
#include "utils/id.h"

#define CANVAS_MAX_FRAMES 1024

typedef enum {
  CanvasStatus_Success,
  CanvasStatus_UndefError,
} CanvasStatus;

typedef struct {
  ALLOCATOR_ID_LIST(CANVAS_MAX_FRAMES) frames;
  ALLOCATOR_ID_LIST(CANVAS_MAX_FRAMES) octagons;
  ALLOCATOR_ID_LIST(CANVAS_MAX_FRAMES) selected_frames;
} Canvas;

EXTERN_C_BEGIN


Frame *canvas_create_frame(Canvas *);
Octagon *canvas_create_octagon(Canvas *);

void canvas_draw_callback(Renderer *, void *);

EXTERN_C_END
#endif
