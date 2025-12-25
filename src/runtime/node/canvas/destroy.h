#ifndef _CANVAS_DESTROY_H_
#define _CANVAS_DESTROY_H_

#include "runtime/manager/module.h"
#include "runtime/node/canvas/core.h"
#include "runtime/node/frame.h"

EXTERN_C_BEGIN

CanvasStatus canvas_destroy_frame(Canvas *, Frame *);
CanvasStatus canvas_destroy_module(Canvas *, Frame *);
CanvasStatus canvas_destroy_pod(Canvas *, Frame *);
CanvasStatus canvas_destroy_connector(Canvas *, Connector *);
CanvasStatus canvas_destroy_octagon(Canvas *, Octagon *);
CanvasStatus canvas_destroy_all_selected_frames(Canvas *);

EXTERN_C_END

#endif
