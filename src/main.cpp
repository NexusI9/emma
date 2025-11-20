#include "nkengine/include/context.h"
#include "nkengine/include/gui.h"
#include "nkengine/include/renderer.h"
#include "nkengine/include/resource_manager.h"
#include "runtime/allocator.h"
#include "runtime/canvas/core.h"
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

  Canvas canvas;

  Frame *frame = canvas_create_frame(&canvas);
  Octagon *oct = canvas_create_octagon(&canvas);

  octagon_set_outer_offset(oct, 3, 1.0f);
  octagon_set_outer_offset(oct, 1, 0.6f);
  octagon_set_outer_offset(oct, 2, 0.2f);

  static const char *octalysis_labels[OCTAGON_VERTEX_COUNT] = {
      "Epic Meaning", "Empowerment", "Social Influence", "Unpredictability",
      "Avoidance",    "Scarcity",    "Ownership",        "Accomplishment",
  };

  octagon_update_vertices(oct);
  octagon_set_labels(oct, octalysis_labels);
  octagon_update_labels_coordinates(oct);

  
  Widget::CanvasShape canvas_shape = Widget::CanvasShape(gui, &canvas);
  canvas_shape.update_frame_shapes();

  renderer_add_draw_callback(renderer, canvas_draw_callback, &canvas_shape,
                             RendererDrawMode_All);

  renderer_draw(renderer);

  return 0;
}
