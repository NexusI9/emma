#ifndef _CANVAS_CONNECTION_H_
#define _CANVAS_CONNECTION_H_

#include "runtime/node/canvas/core.h"

EXTERN_C_BEGIN

void canvas_connection_records_init(Canvas *);
void canvas_connection_records_insert(Canvas *, alloc_id, alloc_id, alloc_id);
void canvas_connection_records_clear(Canvas *, alloc_id);

CanvasStatus canvas_connect_frames(Canvas *, Frame *, Frame *, Connector *);
CanvasStatus canvas_disconnect_frames(Canvas *, Frame *, Frame *);

CanvasStatus canvas_get_connector_frames(Canvas *, const Connector *, Frame **,
                                         Frame **);

const CanvasConnectionRecord *
canvas_get_connector_frames_fast(Canvas *, const Connector *);

EXTERN_C_END

#endif
