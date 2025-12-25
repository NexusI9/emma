#include "core.h"
#include "runtime/solutions/persona/config.h"

CanvasStatus canvas_create(Canvas *canvas) { return CanvasStatus_Success; }

static inline void canvas_get_closest_connector_handles(const Frame *,
                                                        const Frame *,
                                                        ConnectorHandle **,
                                                        ConnectorHandle **,
                                                        const int);

CanvasStatus canvas_add_pod_persona(Canvas *canvas, Frame *pod,
                                    const PersonaType type) {

  Frame *frame = new_frame();

  if (!frame) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  Solution *solution = new_solution();
  Motivation *motivation = new_motivation();

  if (solution)
    frame_add_solution(frame, solution->id);

  if (motivation)
    frame_add_motivation(frame, motivation->id);

  if (motivation && solution)
    solution_persona_init(solution, &SOLUTION_PERSONAS[type]);

  const alloc_id window_id = pod->children.entries[pod->children.count - 1];
  const TextureAtlasRegion *sprite = persona_get_sprite(type);

  vec2 position;
  float size, rot;

  persona_create_rand_coordinate(
      pod->world_position, pod->end_point, PERSONA_PADDING,
      (vec2){PERSONA_BASE_SIZE * PERSONA_MIN_RADIUS,
             PERSONA_BASE_SIZE * PERSONA_MAX_RADIUS},
      (vec2){PERSONA_MIN_ANGLE, PERSONA_MAX_RADIUS}, position, &size, &rot);

  frame_create(frame, &(FrameDescriptor){
                          .uv0 = {sprite->uv0[0], sprite->uv0[1]},
                          .uv1 = {sprite->uv1[0], sprite->uv1[1]},
                          .label = sprite->label,
                          .clickbox = &FRAME_CLICKBOX_TYPE_DEFAULT,
                          .position = {position[0], position[1]},
                          .size = {size, size},
                      });

  if (frame_add_child(pod, frame->id) != StaticListStatus_Success) // ERRHANDLE
    return CanvasStatus_ResourceCreationFail;

  /*
  For the pod, the last child is always the window, so we move it at the end of
  the children array
   */
  pod->children.entries[pod->children.count - 2] = frame->id;
  pod->children.entries[pod->children.count - 1] = window_id;

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
  for (size_t i = 0; i < frame_a->connectors.count; i++) {
    for (size_t j = 0; j < frame_b->connectors.count; j++) {
      if (frame_a->connectors.entries[i] == frame_b->connectors.entries[j])
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
      .color = emma_color(ThemeEmmaColor_Border_Subtle_On_Dark),
      .thickness = CONNECTOR_THICKNESS,
      .start = closest_handle_a,
      .end = closest_handle_b,
  };
  connector_create(connector, &cn_desc);

  allocator_id_list_push(canvas->connectors->entries,
                         allocator_connector_capacity(),
                         &canvas->connectors->count, connector->id);

  frame_register_connector(frame_a, connector->id);
  frame_register_connector(frame_b, connector->id);
}

void canvas_disconnect_frames(Canvas *canvas, const Frame *frame_a,
                              const Frame *frame_b) {}

/**
   Add an EXISTING module to a frame
 */
CanvasStatus canvas_add_module_to_frame(Canvas *canvas, Frame *frame,
                                        Frame *module) {

  frame_add_child(frame, module->id);

  frame_update_motivation_from_children(frame, FrameMotivations_Default,
                                        FrameMotivations_Default);
  frame_update_octagon_motivation(frame, FrameMotivations_Default);

  return CanvasStatus_Success;
}

CanvasStatus canvas_remove_module_from_frame(Canvas *canvas, Frame *frame,
                                             Frame *module) {

  frame_remove_child(frame, module->id);

  frame_update_motivation_from_children(frame, FrameMotivations_Default,
                                        FrameMotivations_Default);

  frame_update_octagon_motivation(frame, FrameMotivations_Default);

  return CanvasStatus_Success;
}
