#include "create.h"
#include "core.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/node/frame.h"
#include "runtime/node/motivation.h"
#include "runtime/solutions/module/config.h"
#include "runtime/systems/solution_system.h"
#include "state.h"
#include "transform.h"

static const char *octalysis_labels[OCTAGON_VERTEX_COUNT] = {
    "Epic Meaning", "Empowerment", "Social Influence", "Unpredictability",
    "Avoidance",    "Scarcity",    "Ownership",        "Accomplishment",
};

typedef enum {
  CanvasFrameCreateFlags_None = 0,
  CanvasFrameCreateFlags_CreateOctagon = 1 << 0,
  CanvasFrameCreateFlags_CreateConnectorHandle = 1 << 1,
  CanvasFrameCreateFlags_CreateSolution = 1 << 2,
  CanvasFrameCreateFlags_CreateMotivation = 1 << 3,
  CanvasFrameCreateFlags_All = ~0,
} CanvasFrameCreateFlags;

static inline Frame *canvas_create_frame_core(Canvas *, const FrameDescriptor *,
                                              alloc_id *, const size_t,
                                              size_t *,
                                              const CanvasFrameCreateFlags);

Frame *canvas_create_frame_core(Canvas *canvas,
                                const FrameDescriptor *frame_desc,
                                alloc_id *list_entries, const size_t capacity,
                                size_t *list_count,
                                const CanvasFrameCreateFlags flags) {

  if (*list_entries == capacity)
    return NULL;

  Frame *frame = new_frame();

  if (!frame) // ERRHANDLE
    return NULL;

  {
    // === Frame setup ===
    allocator_id_list_push(list_entries, capacity, list_count, frame->id);
    frame_create(frame, frame_desc);
  }

  if (flags & CanvasFrameCreateFlags_CreateOctagon) {

    // === Octagon ===
    Octagon *oct = canvas_create_octagon(canvas);
    octagon_update_vertices(oct);

    octagon_set_labels(oct, octalysis_labels);
    octagon_update_labels_coordinates(oct);

    // Link octagon to frame
    frame_set_octagon_id(frame, oct->id);
    canvas_align_octagon_to_frame(canvas, frame);

    // add it to draw list
    canvas_register_frame_state(canvas, frame, CanvasFrameState_Octagon);
  }

  if (flags & CanvasFrameCreateFlags_CreateConnectorHandle) {
    // === Connector Handles ===
    for (uint8_t i = 0; i < FRAME_CONNECTOR_HANDLE_COUNT; i++) {
      ConnectorHandle *handle = canvas_create_connector_handle(canvas);
      frame_set_connector_handle_id(frame, (ConnectorHandleSide)(1 << i),
                                    handle->id);
    }

    canvas_align_connector_handle_group_to_frame(canvas, frame);
  }

  if (flags & CanvasFrameCreateFlags_CreateSolution) {
    Solution *solution = new_solution();

    if (solution)
      frame_add_solution(frame, solution->id);
  }

  if (flags & CanvasFrameCreateFlags_CreateMotivation) {

    Motivation *motivation = new_motivation();

    if (motivation)
      frame_add_motivation(frame, motivation->id);
  }

  return frame;
}

Frame *canvas_create_frame(Canvas *canvas) {

  FrameDescriptor frame_desc = {
      .background = emma_color(ThemeEmmaColor_Surface_Highest),
      .position = {200, 200},
      .size = {FRAME_MIN_SIZE, FRAME_MIN_SIZE},
      // TODO: replace the boundbox descriptor by a discriminator
      // "SelectionType" to automatically setup the right boundbox configuration
      // within the frame constructor ?
      .clickbox =
          &(FrameClickboxDescriptor){
              .update_callback = boundbox_frame_update,
              .padding = FRAME_CLICKBOX_THICKNESS,
              .count = BOUNDBOX_FRAME_RECT_COUNT,
          },
  };

  Frame *frame = canvas_create_frame_core(
      canvas, &frame_desc, canvas->frames[CanvasFrameState_Default].entries,
      allocator_frame_capacity(),
      &canvas->frames[CanvasFrameState_Default].count,
      CanvasFrameCreateFlags_CreateOctagon |
          CanvasFrameCreateFlags_CreateMotivation |
          CanvasFrameCreateFlags_CreateConnectorHandle);

  return frame;
}

Frame *canvas_create_module(Canvas *canvas, const ModuleType module) {

  const TextureAtlasRegion *module_desc = get_module(module);

  FrameDescriptor frame_desc = {
      .uv0 = {module_desc->uv0[0], module_desc->uv0[1]},
      .uv1 = {module_desc->uv1[0], module_desc->uv1[1]},
      .size = {module_desc->size[0], module_desc->size[1]},
      .label = module_desc->label,
      .clickbox =
          &(FrameClickboxDescriptor){
              .update_callback = boundbox_update,
              .padding = -FRAME_CLICKBOX_THICKNESS / 2.0f,
              .count = 1,
          },
  };

  Frame *frame = canvas_create_frame_core(
      canvas, &frame_desc, canvas->modules[CanvasModuleState_Default].entries,
      allocator_frame_capacity(),
      &canvas->modules[CanvasModuleState_Default].count,
      CanvasFrameCreateFlags_CreateConnectorHandle |
          CanvasFrameCreateFlags_CreateSolution |
          CanvasFrameCreateFlags_CreateMotivation);

  // === create motivation ===
  if (frame) {

    Motivation *motivation = allocator_motivation_entry(
        frame->motivations.entries[frame->motivations.count - 1]);

    Solution *solution = allocator_solution_entry(
        frame->solutions.entries[frame->solutions.count - 1]);

    if (motivation && solution) {
      solution_module_init(solution, &SOLUTION_MODULES[module]);
      solution_system_update_motivation(solution, motivation);
    }
  }

  return frame;
}

Frame *canvas_create_pod(Canvas *canvas) {

  const TextureAtlasRegion *pod_region = ui_sprite(UISprite_Pod_Base);

  FrameDescriptor pod_desc = {
      .uv0 = {pod_region->uv0[0], pod_region->uv0[1]},
      .uv1 = {pod_region->uv1[0], pod_region->uv1[1]},
      .size = {pod_region->size[0], pod_region->size[0]},
      .label = pod_region->label,
      .clickbox = &FRAME_CLICKBOX_TYPE_DEFAULT,
  };

  Frame *pod = canvas_create_frame_core(
      canvas, &pod_desc, canvas->pods[CanvasPodState_Default].entries,
      allocator_frame_capacity(), &canvas->pods[CanvasPodState_Default].count,
      CanvasFrameCreateFlags_CreateConnectorHandle |
          CanvasFrameCreateFlags_CreateSolution);

  Frame *pod_window = new_frame();

  if (pod_window) {

    const TextureAtlasRegion *window_region = ui_sprite(UISprite_Pod_Window);

    frame_create(pod_window,
                 &(FrameDescriptor){
                     .uv0 = {window_region->uv0[0], window_region->uv0[1]},
                     .uv1 = {window_region->uv1[0], window_region->uv1[1]},
                     .size = {window_region->size[0], window_region->size[1]},
                     .label = window_region->label,
                     .clickbox = &FRAME_CLICKBOX_TYPE_DEFAULT,
                 });

    frame_add_child(pod, pod_window->id);
  }

  return pod;
}

Octagon *canvas_create_octagon(Canvas *canvas) {

  const size_t capacity = allocator_octagon_capacity();

  if (canvas->octagons.count == capacity)
    return NULL;

  Octagon *oct = new_octagon();

  if (!oct) // ERRHANDLE
    return NULL;

  allocator_id_list_push(canvas->octagons.entries, capacity,
                         &canvas->octagons.count, oct->id);

  OctagonDescriptor oct_desc = {
      .inner_color = {0.1f, 0.1, 0.1f, 1.0f},
      .outer_color = {1.0f, 1.0f, 1.0f, 1.0f},
      .inner_radius = 0.6f,
      .label = "Octagon",
      .position =
          {
              context_width() / 2.0f * (float)context_dpi(),
              context_height() / 2.0f * (float)context_dpi(),
          },
      .scale = OCTAGON_BASE_SCALE,
  };

  octagon_create(oct, &oct_desc);

  return oct;
}

ConnectorHandle *canvas_create_connector_handle(Canvas *canvas) {

  const size_t capacity = allocator_connector_handle_capacity();

  if (canvas->connector_handles.count == capacity)
    return NULL;

  ConnectorHandle *handle = new_connector_handle();

  if (!handle) // ERRHANDLE
    return NULL;

  allocator_id_list_push(canvas->connector_handles.entries, capacity,
                         &canvas->connector_handles.count, handle->id);

  ConnectorHandleDescriptor handle_desc = {
      .position = {0.0f, 0.0f},
      .scale = 10.0f,
      .color = emma_color(ThemeEmmaColor_Border_Subtle_On_Dark),
  };
  connector_handle_create(handle, &handle_desc);

  return handle;
}

Connector *canvas_create_connector(Canvas *canvas,
                                   const ConnectorDescriptor *desc) {

  Connector *connector = new_connector();

  if (!connector)
    return NULL;

  connector_create(connector, desc);

  if (allocator_id_list_push(canvas->connectors->entries,
                             allocator_connector_capacity(),
                             &canvas->connectors->count,
                             connector->id) != StaticListStatus_Success)
    return NULL;

  return connector;
}

/**
   Instantiate a new module in frame from it type at a given position.
 */
CanvasStatus canvas_create_module_in_frame(Canvas *canvas, Frame *frame,
                                           const ModuleType type,
                                           const vec2 position) {

  Frame *module = canvas_create_module(canvas, type);

  if (!module) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  if (frame_add_child(frame, module->id) !=
      StaticListStatus_Success) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  canvas_set_module_local_position(canvas, module, position);

  frame_update_motivation_from_children(frame, FrameMotivations_Default,
                                        FrameMotivations_Default);
  frame_update_octagon_motivation(frame, FrameMotivations_Default);

  return CanvasStatus_Success;
}
