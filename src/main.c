#include "nkengine/include/context.h"
#include "nkengine/include/gui.h"
#include "nkengine/include/renderer.h"
#include "nkengine/include/resource_manager.h"
#include "runtime/allocator.h"
#include "runtime/canvas/core.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include <emscripten/emscripten.h>

int main() {

  context_init(&(ContextDescriptor){
      .html_target = "canvas",
  });

  Renderer *renderer = rem_new_renderer();
  renderer_create(renderer, &(RendererCreateDescriptor){
                                .background = {1.0, 0.1, 0.1, 1.0f},
                                .dpi = RENDERER_DPI_AUTO,
                                .width = RENDERER_WIDTH_AUTO,
                                .height = RENDERER_HEIGHT_AUTO,
                            });

  Gui *gui = rem_new_gui();
  gui_init(gui, &(GuiDescriptor){
                    .dpi = context_dpi(),
                    .theme = &g_theme,
                    .renderer = renderer,
                    .active_scene = NULL,
                });

  allocator_init();

  Canvas canvas;
  Frame *frame = canvas_create_frame(&canvas);
  Octagon *oct = canvas_create_octagon(&canvas);

  octagon_set_outer_offset(oct, 3, 1.0f);
  octagon_set_outer_offset(oct, 1, 0.6f);
  octagon_set_outer_offset(oct, 2, 0.2f);

  octagon_update_vertices(oct);

  CanvasDrawData draw_data = {.gui = gui, .canvas = &canvas};
  renderer_add_draw_callback(renderer, canvas_draw_callback, (void *)&draw_data,
                             RendererDrawMode_All);

  renderer_draw(renderer);

  return 0;
}
