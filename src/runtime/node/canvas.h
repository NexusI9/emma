#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "nkengine/include/gui.h"
#include "nkengine/include/renderer.h"
#include "runtime/manager/allocator.h"
#include "runtime/node/connector_handle.h"
#include "runtime/node/frame.h"
#include "utils/id.h"

#define CANVAS_MAX_FRAMES 1024

typedef enum {
  CanvasFrameState_Default,
  CanvasFrameState_Octagon,
  CanvasFrameState_Highlighted,
  CanvasFrameState_Selected,
  CanvasFrameState_COUNT,
} CanvasFrameState;

typedef enum {
  CanvasModuleState_Default,
  CanvasModuleState_Selected,
  CanvasModuleState_COUNT,
} CanvasModuleState;

typedef enum {
  CanvasStatus_Success,
  CanvasStatus_UndefError,
} CanvasStatus;

typedef struct {
  ALLOCATOR_ID_LIST(CANVAS_MAX_FRAMES) frames[CanvasFrameState_COUNT];
  ALLOCATOR_ID_LIST(CANVAS_MAX_FRAMES) modules[CanvasModuleState_COUNT];
  ALLOCATOR_ID_LIST(CANVAS_MAX_FRAMES) octagons;
  ALLOCATOR_ID_LIST(CANVAS_MAX_FRAMES) connectors;
  ALLOCATOR_ID_LIST(CANVAS_MAX_FRAMES *CONNECTOR_HANDLE_COUNT)
  connector_handles;
} Canvas;

EXTERN_C_BEGIN

Frame *canvas_create_frame(Canvas *);
Octagon *canvas_create_octagon(Canvas *);
ConnectorHandle *canvas_create_connector_handle(Canvas *);


CanvasStatus canvas_add_module_to_frame(Canvas *, Frame *, const alloc_id);

void canvas_align_octagon_to_frame(Canvas *, const Frame *);
void canvas_align_connector_handle_group_to_frame(Canvas *, const Frame *);
void canvas_update_frame_connectors(Canvas *, const Frame *);

void canvas_set_frame_position(Canvas *, Frame *, const vec2);
void canvas_set_frame_size(Canvas *, Frame *, const vec2);

StaticListStatus canvas_register_frame_state(Canvas *, const Frame *,
                                             const CanvasFrameState);

StaticListStatus canvas_unregister_frame_state(Canvas *, const Frame *,
                                               const CanvasFrameState);

StaticListStatus canvas_empty_frame_state(Canvas *, const CanvasFrameState);

void canvas_connect_frames(Canvas *, Frame *, Frame *);
void canvas_disconnect_frames(Canvas *, Frame *, Frame *);

void canvas_draw_callback(Renderer *, void *);

EXTERN_C_END
#endif
