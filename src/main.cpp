#include "nkengine/include/context.h"
#include "nkengine/include/gui.h"
#include "nkengine/include/renderer.h"
#include "nkengine/include/resource_manager.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/unit.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/canvas.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/widgets/canvas.hpp"
#include <emscripten/emscripten.h>

int main() {

  ContextDescriptor ctx = {.html_target = "canvas"};
  context_init(&ctx);

  Renderer *renderer = rem_new_renderer();
  RendererCreateDescriptor rd = {
      .background = {1.0, 0.1, 0.1, 1.0f},
      .dpi = RENDERER_DPI_AUTO,
      .width = RENDERER_WIDTH_AUTO,
      .height = RENDERER_HEIGHT_AUTO,
  };
  renderer_create(renderer, &rd);

  Gui *gui = rem_new_gui();
  GuiDescriptor g = {
      .dpi = context_dpi(),
      .theme = &g_theme,
      .renderer = renderer,
      .active_scene = NULL,
  };
  gui_init(gui, &g);

  allocator_init();
  viewport_set_pan_sensitivity(20.0f);
  viewport_set_zoom_sensitivity(0.001f);
  unit_set_step(20.0f);

  Canvas canvas;

  // === Frame Generation ===
  static const vec2 frames[] = {
      {0.0f, 0.0f},
      {1200.0f, 400.0f},
      {2300.0f, -300.0f},
  };
  static const int frames_count = sizeof(frames) / sizeof(frames[0]);

  for (uint8_t i = 0; i < frames_count; i++) {
    Frame *frame = canvas_create_frame(&canvas);
    canvas_set_frame_position(&canvas, frame, frames[i]);
    canvas_register_frame_state(&canvas, frame, CanvasFrameState_Octagon);

    if (i > 0) {
      Frame *previous_frame = allocator_frame_entry(
          canvas.frames[CanvasFrameState_Default].entries[i - 1]);
      canvas_connect_frames(&canvas, previous_frame, frame);
    }
  }

  Widget::CanvasShape canvas_shape = Widget::CanvasShape(gui, &canvas);
  canvas_shape.sync_shapes();

  renderer_add_draw_callback(renderer, canvas_draw_callback, &canvas_shape,
                             RendererDrawMode_All);

  renderer_draw(renderer);

  return 0;
}
