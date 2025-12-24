#ifndef _CANVAS_H_
#define _CANVAS_H_

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

typedef enum {
  CanvasStatus_Success,
  CanvasStatus_ResourceCreationFail,
  CanvasStatus_NothingSelected,
  CanvasStatus_UndefError,
} CanvasStatus;

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
Frame *canvas_create_frame(Canvas *);
Frame *canvas_create_module(Canvas *, const ModuleType);
Frame *canvas_create_pod(Canvas *);
Octagon *canvas_create_octagon(Canvas *);
ConnectorHandle *canvas_create_connector_handle(Canvas *);
Connector *canvas_create_connector(Canvas *, const ConnectorDescriptor *);

CanvasStatus canvas_add_module_to_frame(Canvas *, Frame *, const ModuleType,
                                        const vec2);

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
CanvasStatus canvas_add_pod_persona(Canvas *, Frame *, const PersonaType);

StaticListStatus canvas_register_frame_state(Canvas *, const Frame *,
                                             const CanvasFrameState);

StaticListStatus canvas_unregister_frame_state(Canvas *, const Frame *,
                                               const CanvasFrameState);

StaticListStatus canvas_empty_frame_state(Canvas *, const CanvasFrameState);
StaticListStatus canvas_empty_module_state(Canvas *, const CanvasModuleState);
StaticListStatus canvas_empty_pod_state(Canvas *, const CanvasPodState);

void canvas_connect_frames(Canvas *, Frame *, Frame *);

void canvas_disconnect_frames(Canvas *, const Frame *, const Frame *);

CanvasStatus canvas_destroy_frame(Canvas *, Frame *);
CanvasStatus canvas_destroy_module(Canvas *, Frame *);
CanvasStatus canvas_destroy_pod(Canvas *, Frame *);
CanvasStatus canvas_destroy_connector(Canvas *, Connector *);
CanvasStatus canvas_destroy_octagon(Canvas *, Octagon *);

CanvasStatus canvas_destroy_all_selected_frames(Canvas *);

EXTERN_C_END
#endif
