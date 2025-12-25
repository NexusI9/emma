#ifndef _CANVAS_TRANSFORM_H_
#define _CANVAS_TRANSFORM_H_

#include "runtime/manager/module.h"
#include "runtime/node/canvas/core.h"
#include "runtime/node/frame.h"


EXTERN_C_BEGIN

void canvas_align_octagon_to_frame(Canvas *, const Frame *);
void canvas_align_connector_handle_group_to_frame(Canvas *, const Frame *);
void canvas_update_frame_connectors(Canvas *, const Frame *);
void canvas_update_connectors_handle_to_frame(Canvas *, const Frame *);

void canvas_set_frame_position(Canvas *, Frame *, const vec2);
void canvas_set_frame_size(Canvas *, Frame *, const vec2);
void canvas_frame_wrap(Canvas *, Frame *);

void canvas_set_module_local_position(Canvas *, Frame *, const vec2);
void canvas_set_module_world_position(Canvas *, Frame *, const vec2);
void canvas_set_module_size(Canvas *, Frame *, const vec2);

void canvas_set_pod_position(Canvas *, Frame *, const vec2);
void canvas_set_pod_size(Canvas *, Frame *, const vec2);

EXTERN_C_END

#endif
