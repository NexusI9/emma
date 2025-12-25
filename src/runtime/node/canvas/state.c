#include "state.h"

StaticListStatus canvas_register_frame_state(Canvas *canvas, const Frame *frame,
                                             const CanvasFrameState state) {

  return allocator_id_list_push(canvas->frames[state].entries,
                                allocator_frame_capacity(),
                                &canvas->frames[state].count, frame->id);
}

StaticListStatus canvas_unregister_frame_state(Canvas *canvas,
                                               const Frame *frame,
                                               const CanvasFrameState state) {

  return allocator_id_list_pop(canvas->frames[state].entries,
                               &canvas->frames[state].count, frame->id);
}

StaticListStatus canvas_empty_frame_state(Canvas *canvas,
                                          const CanvasFrameState state) {
  return stli_empty(canvas->frames[state].entries, &canvas->frames[state].count,
                    sizeof(alloc_id), "Canvas Frame State list");
}

StaticListStatus canvas_empty_module_state(Canvas *canvas,
                                           const CanvasModuleState state) {
  return stli_empty(canvas->modules[state].entries,
                    &canvas->modules[state].count, sizeof(alloc_id),
                    "Canvas Module State list");
}

StaticListStatus canvas_empty_pod_state(Canvas *canvas,
                                        const CanvasPodState state) {
  return stli_empty(canvas->pods[state].entries, &canvas->pods[state].count,
                    sizeof(alloc_id), "Canvas Pod State list");
}
