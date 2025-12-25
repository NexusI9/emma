#ifndef _CANVAS_CREATE_H_
#define _CANVAS_CREATE_H_

#include "runtime/manager/module.h"
#include "runtime/node/canvas/core.h"
#include "runtime/node/frame.h"

EXTERN_C_BEGIN

Frame *canvas_create_frame(Canvas *);
Frame *canvas_create_module(Canvas *, const ModuleType);
Frame *canvas_create_pod(Canvas *);
Octagon *canvas_create_octagon(Canvas *);
ConnectorHandle *canvas_create_connector_handle(Canvas *);
Connector *canvas_create_connector(Canvas *, const ConnectorDescriptor *);

CanvasStatus canvas_create_module_in_frame(Canvas *, Frame *, const ModuleType,
                                           const vec2);

EXTERN_C_END

#endif
