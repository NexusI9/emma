#ifndef _CANVAS_CORE_H_
#define _CANVAS_CORE_H_

#include "nkengine/include/gui.h"
#include "nkengine/include/renderer.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/module.h"
#include "runtime/node/connector.h"
#include "runtime/node/connector_handle.h"
#include "runtime/node/frame.h"
#include "runtime/node/persona.h"
#include "utils/id.h"

typedef enum {
  CanvasStatus_Success,
  CanvasStatus_ResourceCreationFail,
  CanvasStatus_NothingSelected,
  CanvasStatus_UndefError,
} CanvasStatus;

typedef enum {
  CanvasFrameState_Default,
  CanvasFrameState_Octagon,
  CanvasFrameState_Selected,
  CanvasFrameState_COUNT,
} CanvasFrameState;

typedef enum {
  CanvasModuleState_Default,
  CanvasModuleState_Selected,
  CanvasModuleState_COUNT,
} CanvasModuleState;

typedef enum {
  CanvasPodState_Default,
  CanvasPodState_Selected,
  CanvasPodState_COUNT,
} CanvasPodState;

typedef enum {
  CanvasConnectorState_Default,
  CanvasConnectorState_Selected,
  CanvasConnectorState_COUNT,
} CanvasConnectorState;

typedef STATIC_LIST(alloc_id, ALLOCATOR_FRAME_CAPACITY) FrameAllocList;

typedef struct {

  FrameAllocList frames[CanvasFrameState_COUNT];
  FrameAllocList modules[CanvasModuleState_COUNT];
  FrameAllocList pods[CanvasPodState_COUNT];
  FrameAllocList connectors[CanvasConnectorState_COUNT];
  FrameAllocList octagons;

  STATIC_LIST(alloc_id, ALLOCATOR_FRAME_CAPACITY *FRAME_CONNECTOR_HANDLE_COUNT)
  connector_handles;

} Canvas;

EXTERN_C_BEGIN

CanvasStatus canvas_create(Canvas *);

// TODO not sure where to put these ones like in add.h and connect.h
CanvasStatus canvas_add_module_to_frame(Canvas *, Frame *, Frame *);
CanvasStatus canvas_remove_module_from_frame(Canvas *, Frame *, Frame *);

CanvasStatus canvas_add_pod_persona(Canvas *, Frame *, const PersonaType);

void canvas_connect_frames(Canvas *, Frame *, Frame *);
void canvas_disconnect_frames(Canvas *, Frame *, Frame *);

EXTERN_C_END
#endif
