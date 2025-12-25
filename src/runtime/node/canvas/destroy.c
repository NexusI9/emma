#include "destroy.h"


static inline CanvasStatus
canvas_destroy_frame_core(Canvas *canvas, Frame *frame, FrameAllocList *);


/**
   Note that we only handle the frame destruction at N = 1 level, we don't take
   care of destroying the children in this "core" function since the children
   may be modules or other "subtypes" of frames.
 */
CanvasStatus canvas_destroy_frame_core(Canvas *canvas, Frame *frame,
                                       FrameAllocList *list) {

  // remove it from the target canvas list (frame, pod, module...)
  allocator_id_list_pop(list->entries, &list->count, frame->id);

  // Unlink all frames connectors
  for (size_t i = 0; i < frame->connectors.count; i++) {
    Connector *connector =
        allocator_connector_entry(frame->connectors.entries[i]);
    canvas_destroy_connector(canvas, connector);
  }

  // Need to unregister the octagon
  if (frame->octagon != ID_UNDEFINED) {
    Octagon *octagon = allocator_octagon_entry(frame->octagon);
    canvas_destroy_octagon(canvas, octagon);
  }

  // Cache the parent ID cause we may need to recompute the parent frame
  // motivations uppon module deletion.
  const alloc_id temp_parent = frame->parent;

  frame_destroy(frame);

  // Recompute the (cached) parent frame motivation since we just destroyed the
  // child above.
  if (temp_parent != ID_UNDEFINED) {
    Frame *parent = allocator_frame_entry(temp_parent);
    if (parent) {
      frame_update_motivation_from_children(parent, FrameMotivations_Default,
                                            FrameMotivations_Default);
      frame_update_octagon_motivation(parent, FrameMotivations_Default);
    }
  }

  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_octagon(Canvas *canvas, Octagon *octagon) {

  allocator_id_list_pop(canvas->octagons.entries, &canvas->octagons.count,
                        octagon->id);

  // unlink it from each frames sharing this octagon id
  for (size_t i = 0; i < canvas->frames->count; i++) {
    Frame *frame = allocator_frame_entry(canvas->frames->entries[i]);
    if (frame->octagon == octagon->id)
      frame->octagon = ID_UNDEFINED;
  }

  destroy_octagon(octagon->id);

  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_frame(Canvas *canvas, Frame *frame) {

  // unregister it from other state list
  for (uint8_t i = 1; i < CanvasFrameState_COUNT; i++)
    allocator_id_list_pop(canvas->frames[i].entries, &canvas->frames[i].count,
                          frame->id);

  // For frames, we destroy all the inner modules as well
  while (frame->children.count) {
    Frame *child = allocator_frame_entry(
        frame->children.entries[frame->children.count - 1]);
    canvas_destroy_module(canvas, child);
  }

  canvas_destroy_frame_core(canvas, frame, canvas->frames);

  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_module(Canvas *canvas, Frame *frame) {

  // unregister it from other state list
  for (uint8_t i = 1; i < CanvasModuleState_COUNT; i++)
    allocator_id_list_pop(canvas->modules[i].entries, &canvas->modules[i].count,
                          frame->id);

  canvas_destroy_frame_core(canvas, frame, canvas->modules);

  // eventual additional removal to take care of
  return CanvasStatus_Success;
}

CanvasStatus canvas_destroy_pod(Canvas *canvas, Frame *frame) {

  // unregister it from other state list
  for (uint8_t i = 1; i < CanvasPodState_COUNT; i++)
    allocator_id_list_pop(canvas->pods[i].entries, &canvas->pods[i].count,
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

    for (size_t j = 0; j < frame_type[i]->count; j++) {
      Frame *frame = allocator_frame_entry(frame_type[i]->entries[j]);

      for (size_t k = 0; k < frame->connectors.count; k++)
        if (frame->connectors.entries[k] == connector->id)
          frame_unregister_connector(frame, connector->id);
    }
  }

  // make sure to remove it from the state lists
  for (uint8_t i = 0; i < CanvasConnectorState_COUNT; i++)
    allocator_id_list_pop(canvas->connectors[i].entries,
                          &canvas->connectors[i].count, connector->id);

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
          &canvas->frames[CanvasFrameState_Selected].count,
          canvas_destroy_frame,
      },
      {
          canvas->modules[CanvasModuleState_Selected].entries,
          &canvas->modules[CanvasModuleState_Selected].count,
          canvas_destroy_module,
      },
      {
          canvas->pods[CanvasPodState_Selected].entries,
          &canvas->pods[CanvasPodState_Selected].count,
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
