#ifndef _CANVAS_STATE_H_
#define _CANVAS_STATE_H_

#include "runtime/manager/module.h"
#include "runtime/node/canvas/core.h"
#include "runtime/node/frame.h"

EXTERN_C_BEGIN

StaticListStatus canvas_register_frame_state(Canvas *, const Frame *,
                                             const CanvasFrameState);

StaticListStatus canvas_unregister_frame_state(Canvas *, const Frame *,
                                               const CanvasFrameState);

StaticListStatus canvas_empty_frame_state(Canvas *, const CanvasFrameState);
StaticListStatus canvas_empty_module_state(Canvas *, const CanvasModuleState);
StaticListStatus canvas_empty_pod_state(Canvas *, const CanvasPodState);

EXTERN_C_END

#endif
