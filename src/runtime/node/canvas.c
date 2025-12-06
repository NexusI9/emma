#include "canvas.h"
#include "runtime/geometry/boundbox.h"
#include "runtime/geometry/vector.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/manager/module.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/node/connector.h"
#include "runtime/node/connector_handle.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/node/persona.h"
#include "utils/id.h"
#include <stdint.h>
#include <string.h>

static const char *octalysis_labels[OCTAGON_VERTEX_COUNT] = {
    "Epic Meaning", "Empowerment", "Social Influence", "Unpredictability",
    "Avoidance",    "Scarcity",    "Ownership",        "Accomplishment",
};

typedef enum {
  CanvasFrameCreateFlags_None = 0,
  CanvasFrameCreateFlags_CreateOctagon = 1 << 0,
  CanvasFrameCreateFlags_CreateConnectorHandle = 1 << 1,
  CanvasFrameCreateFlags_All = ~0,
} CanvasFrameCreateFlags;

static const float OCTAGON_BASE_SCALE = 100.0f;

static inline CanvasStatus
canvas_destroy_frame_core(Canvas *canvas, Frame *frame, FrameAllocList *);

CanvasStatus canvas_create(Canvas *canvas) { return CanvasStatus_Success; }

static inline void canvas_get_closest_connector_handles(const Frame *,
                                                        const Frame *,
                                                        ConnectorHandle **,
                                                        ConnectorHandle **,
                                                        const int);

static inline Frame *canvas_create_frame_core(Canvas *, const FrameDescriptor *,
                                              alloc_id *, const size_t,
                                              size_t *,
                                              const CanvasFrameCreateFlags);

Frame *canvas_create_frame_core(Canvas *canvas,
                                const FrameDescriptor *frame_desc,
                                alloc_id *list_entries, const size_t capacity,
                                size_t *list_length,
                                const CanvasFrameCreateFlags flags) {

  if (*list_entries == capacity)
    return NULL;

  Frame *frame = new_frame();

  if (!frame) // ERRHANDLE
    return NULL;

  {
    // === Frame setup ===
    allocator_id_list_push(list_entries, capacity, list_length, frame->id);
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
  return frame;
}

Frame *canvas_create_frame(Canvas *canvas) {

  FrameDescriptor frame_desc = {
      .background = {1.0f, 1.0f, 1.0f, 1.0f},
      .position = {200, 200},
      .size = {360, 700},
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

  return canvas_create_frame_core(
      canvas, &frame_desc, canvas->frames[CanvasFrameState_Default].entries,
      allocator_frame_capacity(),
      &canvas->frames[CanvasFrameState_Default].length,
      CanvasFrameCreateFlags_All);
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

  return canvas_create_frame_core(
      canvas, &frame_desc, canvas->modules[CanvasModuleState_Default].entries,
      allocator_frame_capacity(),
      &canvas->modules[CanvasModuleState_Default].length,
      CanvasFrameCreateFlags_CreateConnectorHandle);
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
      allocator_frame_capacity(), &canvas->pods[CanvasPodState_Default].length,
      CanvasFrameCreateFlags_CreateConnectorHandle);

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

  if (canvas->octagons.length == capacity)
    return NULL;

  Octagon *oct = new_octagon();

  if (!oct) // ERRHANDLE
    return NULL;

  allocator_id_list_push(canvas->octagons.entries, capacity,
                         &canvas->octagons.length, oct->id);

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

  if (canvas->connector_handles.length == capacity)
    return NULL;

  ConnectorHandle *handle = new_connector_handle();

  if (!handle) // ERRHANDLE
    return NULL;

  allocator_id_list_push(canvas->connector_handles.entries, capacity,
                         &canvas->connector_handles.length, handle->id);

  ConnectorHandleDescriptor handle_desc = {
      .position = {0.0f, 0.0f},
      .scale = 10.0f,
      .color = {0.6f, 0.6f, 0.6f, 1.0f},
  };
  connector_handle_create(handle, &handle_desc);

  return handle;
}

void canvas_align_octagon_to_frame(Canvas *canvas, const Frame *frame) {

  static const int gap = 100;

  const float *frame_position = frame_get_world_position(frame);
  const float *frame_end_point = frame_get_end_point(frame);

  vec2 top_edge;
  glm_vec2_copy((float *)frame_end_point, top_edge);

  top_edge[1] = frame_position[1]; // align with top edge

  vec2 oct_position;
  vec2_avg_2(frame_position, top_edge, oct_position);

  glm_vec2_sub(oct_position, (vec2){0, OCTAGON_BASE_SCALE + gap}, oct_position);

  Octagon *octagon = allocator_octagon_entry(frame->octagon_id);
  octagon_set_position(octagon, oct_position);
}

/**
   Move the 4 connectors handles according to the frame position.
   Used when we move a frame position.
 */
void canvas_align_connector_handle_group_to_frame(Canvas *canvas,
                                                  const Frame *frame) {

  static const int gap = 40;
  static const vec2 gaps[] = {
      // clang-format off
      [ ConnectorHandleSide_Top    ] = {  0.0f        , -1.0f * gap  },
      [ ConnectorHandleSide_Right  ] = {  1.0f * gap  ,        0.0f  },
      [ ConnectorHandleSide_Bottom ] = {  0.0f        ,  1.0f * gap  },
      [ ConnectorHandleSide_Left   ] = { -1.0f * gap  ,        0.0f  },
      // clang-format on
  };

  BoundboxFrame edges;
  boundbox_edges_from_points(frame->local_position, frame->end_point, edges);

  for (uint8_t i = 0; i < FRAME_CONNECTOR_HANDLE_COUNT; i++) {

    ConnectorHandle *handle =
        allocator_connector_handle_entry(frame->connector_handle_id[i]);

    vec2 pos;
    vec2_avg_2(edges[i].p0, edges[i].p1, pos);
    glm_vec2_add(pos, (float *)gaps[(1 << i)], pos);

    connector_handle_set_position(handle, pos);
  }
}

/**
  Use the cached connectors id of the frame to update update the connector
  handles position accordingly. Used when we move a frame position.
 */
void canvas_update_connectors_handle_to_frame(Canvas *canvas,
                                              const Frame *frame) {

  for (size_t i = 0; i < frame->connectors_id.length; i++) {
    Connector *connector =
        allocator_connector_entry(frame->connectors_id.entries[i]);

    connector_update_handle_position(connector);
  }
}

void canvas_set_frame_position(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_world_position(frame, value);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}

void canvas_set_frame_size(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}

void canvas_frame_wrap(Canvas *canvas, Frame *frame) {
  frame_wrap(frame);
  canvas_align_octagon_to_frame(canvas, frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}

/**
   Used when we add a module to a frame
 */
void canvas_set_module_local_position(Canvas *canvas, Frame *frame,
                                      const vec2 value) {
  frame_set_local_position(frame, value); // !! Relative to parent
  frame_update_world_position(frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

/**
   Used when we transform a module on the canvas
 */
void canvas_set_module_world_position(Canvas *canvas, Frame *frame,
                                      const vec2 value) {
  frame_set_world_position(frame, value);
  frame_update_world_position(frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

void canvas_set_module_size(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
}

void canvas_set_pod_position(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_world_position(frame, value);
  frame_update_world_position(frame);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}

void canvas_set_pod_size(Canvas *canvas, Frame *frame, const vec2 value) {
  frame_set_size(frame, value);
  canvas_align_connector_handle_group_to_frame(canvas, frame);
  canvas_update_connectors_handle_to_frame(canvas, frame);
}

StaticListStatus canvas_register_frame_state(Canvas *canvas, const Frame *frame,
                                             const CanvasFrameState state) {

  return allocator_id_list_push(canvas->frames[state].entries,
                                allocator_frame_capacity(),
                                &canvas->frames[state].length, frame->id);
}

StaticListStatus canvas_unregister_frame_state(Canvas *canvas,
                                               const Frame *frame,
                                               const CanvasFrameState state) {

  return allocator_id_list_pop(canvas->frames[state].entries,
                               &canvas->frames[state].length, frame->id);
}

StaticListStatus canvas_empty_frame_state(Canvas *canvas,
                                          const CanvasFrameState state) {
  return stli_empty(canvas->frames[state].entries,
                    &canvas->frames[state].length, sizeof(alloc_id),
                    "Canvas Frame State list");
}

StaticListStatus canvas_empty_module_state(Canvas *canvas,
                                           const CanvasModuleState state) {
  return stli_empty(canvas->modules[state].entries,
                    &canvas->modules[state].length, sizeof(alloc_id),
                    "Canvas Module State list");
}

StaticListStatus canvas_empty_pod_state(Canvas *canvas,
                                        const CanvasPodState state) {
  return stli_empty(canvas->pods[state].entries, &canvas->pods[state].length,
                    sizeof(alloc_id), "Canvas Pod State list");
}

CanvasStatus canvas_add_pod_persona(Canvas *canvas, Frame *pod,
                                    const PersonaType type) {

  Frame *persona_frame = new_frame();

  if (!persona_frame) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  const alloc_id window_id = pod->children.entries[pod->children.length - 1];
  const TextureAtlasRegion *sprite = persona_get_sprite(type);

  vec2 position;
  float size, rot;

  persona_create_rand_coordinate(
      pod->world_position, pod->end_point, PERSONA_PADDING,
      (vec2){PERSONA_BASE_SIZE * PERSONA_MIN_RADIUS,
             PERSONA_BASE_SIZE * PERSONA_MAX_RADIUS},
      (vec2){PERSONA_MIN_ANGLE, PERSONA_MAX_RADIUS}, position, &size, &rot);

  frame_create(persona_frame, &(FrameDescriptor){
                                  .uv0 = {sprite->uv0[0], sprite->uv0[1]},
                                  .uv1 = {sprite->uv1[0], sprite->uv1[1]},
                                  .label = sprite->label,
                                  .clickbox = &FRAME_CLICKBOX_TYPE_DEFAULT,
                                  .position = {position[0], position[1]},
                                  .size = {size, size},
                              });

  if (frame_add_child(pod, persona_frame->id) !=
      StaticListStatus_Success) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  /*
  For the pod, the last child is always the window, so we move it at the end of
  the children array
   */
  pod->children.entries[pod->children.length - 2] = persona_frame->id;
  pod->children.entries[pod->children.length - 1] = window_id;

  return CanvasStatus_Success;
}

void canvas_get_closest_connector_handles(const Frame *frame_a,
                                          const Frame *frame_b,
                                          ConnectorHandle **closest_handle_a,
                                          ConnectorHandle **closest_handle_b,
                                          const int side) {

  float closest_distance = FLT_MAX;
  for (uint8_t i = 0; i < FRAME_CONNECTOR_HANDLE_COUNT; i++) {

    // skipping unwanted sides
    if ((__builtin_ctz(side) & i) == 0)
      continue;

    ConnectorHandle *handle_a =
        allocator_connector_handle_entry(frame_a->connector_handle_id[i]);

    for (uint8_t j = 0; j < FRAME_CONNECTOR_HANDLE_COUNT; j++) {

      // skipping unwanted sides
      if ((__builtin_ctz(side) & j) == 0)
        continue;

      ConnectorHandle *handle_b =
          allocator_connector_handle_entry(frame_b->connector_handle_id[j]);

      float distance =
          glm_vec2_distance(handle_a->position, handle_b->position);

      if (distance < closest_distance) {
        closest_distance = distance;
        *closest_handle_a = handle_a;
        *closest_handle_b = handle_b;
      }
    }
  }
}

void canvas_connect_frames(Canvas *canvas, Frame *frame_a, Frame *frame_b) {

  Connector *connector = new_connector();
  if (!connector) // ERRHANDLE
    return;

  // make sure it's not connected yet
  for (size_t i = 0; i < frame_a->connectors_id.length; i++) {
    for (size_t j = 0; j < frame_b->connectors_id.length; j++) {
      if (frame_a->connectors_id.entries[i] ==
          frame_b->connectors_id.entries[j])
        return;
    }
  }

  // retrieve the closest handle position
  ConnectorHandle *closest_handle_a, *closest_handle_b;
  canvas_get_closest_connector_handles(
      frame_a, frame_b, &closest_handle_a, &closest_handle_b,
      ConnectorHandleSide_Left | ConnectorHandleSide_Right);

  // create connector
  ConnectorDescriptor cn_desc = {
      .color = CONNECTOR_COLOR,
      .thickness = CONNECTOR_THICKNESS,
      .start = closest_handle_a,
      .end = closest_handle_b,
  };
  connector_create(connector, &cn_desc);

  allocator_id_list_push(canvas->connectors->entries,
                         allocator_connector_capacity(),
                         &canvas->connectors->length, connector->id);

  frame_register_connector(frame_a, connector->id);
  frame_register_connector(frame_b, connector->id);
}

Connector *canvas_create_connector(Canvas *canvas,
                                   const ConnectorDescriptor *desc) {

  Connector *connector = new_connector();

  if (!connector)
    return NULL;

  connector_create(connector, desc);

  if (allocator_id_list_push(canvas->connectors->entries,
                             allocator_connector_capacity(),
                             &canvas->connectors->length,
                             connector->id) != StaticListStatus_Success)
    return NULL;

  return connector;
}

void canvas_disconnect_frames(Canvas *canvas, const Frame *frame_a,
                              const Frame *frame_b) {}

/**
   Recompute the frame's registered connectors corners. Useful when we transform
   a frame.
 */
void canvas_update_frame_connectors(Canvas *canvas, const Frame *frame) {

  for (size_t i = 0; i < frame->connectors_id.length; i++) {
    Connector *connector =
        allocator_connector_entry(frame->connectors_id.entries[i]);

    connector_update_corners(connector);
    connector_update_clickboxes(connector, CONNECTOR_CLICKBOX_THICKNESS);
  }
}

CanvasStatus canvas_add_module_to_frame(Canvas *canvas, Frame *frame,
                                        const ModuleType type,
                                        const vec2 position) {

  Frame *module = canvas_create_module(canvas, type);

  if (!module) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  if (frame_add_child(frame, module->id) !=
      StaticListStatus_Success) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  canvas_set_module_local_position(canvas, module, position);

  return CanvasStatus_Success;
}

/**
   Note that we only handle the frame destruction at N = 1 level, we don't take
   care of destroying the children in this "core" function since the children
   may be modules or other "subtypes" of frames.
 */
CanvasStatus canvas_destroy_frame_core(Canvas *canvas, Frame *frame,
                                       FrameAllocList *list) {

  // remove it from the target canvas list (frame, pod, module...)
  allocator_id_list_pop(list->entries, &list->length, frame->id);

  // Unlink all frames connectors
  for (size_t i = 0; i < frame->connectors_id.length; i++) {
    Connector *connector =
        allocator_connector_entry(frame->connectors_id.entries[i]);
    canvas_destroy_connector(canvas, connector);
  }

  frame_destroy(frame);

  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_octagon(Canvas *canvas, Octagon *octagon) {

  for (size_t i = 0; i < canvas->frames->length; i++) {
    Frame *frame = allocator_frame_entry(canvas->frames->entries[i]);
    if (frame->octagon_id == octagon->id)
      frame->octagon_id = ID_UNDEFINED;
  }

  destroy_octagon(octagon->id);

  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_frame(Canvas *canvas, Frame *frame) {

  // unregister it from other state list
  for (uint8_t i = 1; i < CanvasFrameState_COUNT; i++)
    allocator_id_list_pop(canvas->frames[i].entries, &canvas->frames[i].length,
                          frame->id);

  // For frames, we destroy all the inner modules as well
  for (size_t i = 0; i < frame->children.length; i++) {
    Frame *child = allocator_frame_entry(frame->children.entries[i]);
    canvas_destroy_module(canvas, child);
  }
  frame->children.length = 0;

  canvas_destroy_frame_core(canvas, frame, canvas->frames);

  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_module(Canvas *canvas, Frame *frame) {

  // DEBUG
  printf("selected length: %lu\n",
         canvas->modules[CanvasModuleState_Selected].length);
  printf("frame: %p | %llu\n", frame, frame->id);

  // unregister it from other state list
  for (uint8_t i = 1; i < CanvasModuleState_COUNT; i++) {
    allocator_id_list_pop(canvas->modules[i].entries,
                          &canvas->modules[i].length, frame->id);
  }

  canvas_destroy_frame_core(canvas, frame, canvas->modules);

  // eventual additional removal to take care of
  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_pod(Canvas *canvas, Frame *frame) {

  // unregister it from other state list
  for (uint8_t i = 1; i < CanvasPodState_COUNT; i++)
    allocator_id_list_pop(canvas->pods[i].entries, &canvas->pods[i].length,
                          frame->id);

  canvas_destroy_frame_core(canvas, frame, canvas->pods);
  // eventual additional removal to take care of
  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_connector(Canvas *canvas, Connector *connector) {

  FrameAllocList *frame_type[] = {canvas->frames, canvas->pods};

  static const uint8_t select_count =
      sizeof(frame_type) / sizeof(frame_type[0]);

  // remove it from each frame that registered this connector
  for (uint8_t i = 0; i < select_count; i++) {

    for (size_t j = 0; j < frame_type[i]->length; j++) {
      Frame *frame = allocator_frame_entry(frame_type[i]->entries[j]);

      for (size_t k = 0; k < frame->connectors_id.length; k++)
        if (frame->connectors_id.entries[k] == connector->id)
          frame_unregister_connector(frame, connector->id);
    }
  }

  // make sure to remove it from the state lists
  for (uint8_t i = 0; i < CanvasConnectorState_COUNT; i++)
    allocator_id_list_pop(canvas->connectors[i].entries,
                          &canvas->connectors[i].length, connector->id);

  connector_destroy(connector);

  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_all_selected_frames(Canvas *canvas) {

  CanvasStatus status = CanvasStatus_NothingSelected;

  struct {
    alloc_id *entries;
    size_t *count;
    CanvasStatus (*destroyer)(Canvas *, Frame *);
  } select_type[] = {

      {
          canvas->frames[CanvasFrameState_Selected].entries,
          &canvas->frames[CanvasFrameState_Selected].length,
          canvas_destroy_frame,
      },
      {
          canvas->modules[CanvasModuleState_Selected].entries,
          &canvas->modules[CanvasModuleState_Selected].length,
          canvas_destroy_module,
      },
      {
          canvas->pods[CanvasPodState_Selected].entries,
          &canvas->pods[CanvasPodState_Selected].length,
          canvas_destroy_pod,
      }

  };

  static const uint8_t select_count =
      sizeof(select_type) / sizeof(select_type[0]);

  for (uint8_t i = 0; i < select_count; i++) {
    for (size_t j = 0; j < *select_type[i].count; j++) {
      Frame *frame = allocator_frame_entry(select_type[i].entries[j]);
      select_type[i].destroyer(canvas, frame);
      status = CanvasStatus_Success;
    }
  }

  return status;
}
