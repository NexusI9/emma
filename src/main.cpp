#include "nkengine/include/context.h"
#include "nkengine/include/gui.h"
#include "nkengine/include/renderer.h"
#include "nkengine/include/resource_manager.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/module.h"
#include "runtime/manager/unit.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/canvas.h"
#include "runtime/node/frame.h"
#include "runtime/node/octagon.h"
#include "runtime/widgets/canvas.hpp"
#include <emscripten/emscripten.h>

void create_frames(Canvas *canvas) {
  static const struct {
    vec2 position;

    struct {
      const uint8_t index;
      const float value;
    } octagon_data[3];

    struct {

      struct {
        const ModuleType type;
        const vec2 position;
      } entries[3];

      uint8_t count;

    } modules;

  } frames[] = {
      {
          .position = {0.0f, 0.0f},
          .octagon_data = {{1, 0.5}, {6, 0.6}, {2, 0.9}},
          .modules =
              {
                  .entries =
                      {
                          {ModuleType_News, {0.0f, 0.0f}},
                          {ModuleType_Rate, {0.0f, 460.0f}},
                      },
                  .count = 2,
              },
      },
      {
          .position = {1200.0f, 400.0f},
          .octagon_data = {{7, 0.8}, {0, 0.1}, {5, 0.35}},
          .modules =
              {
                  .entries =
                      {
                          {ModuleType_SpinningWheel, {0.0f, 0.0f}},
                      },
                  .count = 1,
              },
      },
      {
          .position = {2300.0f, -300.0f},
          .octagon_data = {{3, 0.9}, {4, 0.6}, {5, 0.7}},
          .modules =
              {
                  .entries =
                      {
                          {ModuleType_Ranking, {0.0f, 0.0f}},
                          {ModuleType_SurveyCheckbox, {0.0f, 320.0f}},
                          {ModuleType_SurveySlider, {0.0f, 520.0f}},
                      },
                  .count = 3,
              },
      },
  };
  static const int frames_count = sizeof(frames) / sizeof(frames[0]);

  for (uint8_t i = 0; i < frames_count; i++) {
    Frame *frame = canvas_create_frame(canvas);
    unit_snap_vec2((float *)frames[i].position);
    canvas_set_frame_position(canvas, frame, frames[i].position);
    canvas_register_frame_state(canvas, frame, CanvasFrameState_Octagon);

    // add octagons
    Octagon *oct = allocator_octagon_entry(frame->octagon_id);
    for (uint8_t j = 0; j < 3; j++) {
      octagon_set_outer_offset(oct, frames[i].octagon_data[j].index,
                               frames[i].octagon_data[j].value);
    }

    // add modules
    for (uint8_t k = 0; k < frames[i].modules.count; k++)
      canvas_add_module_to_frame(canvas, frame,
                                 frames[i].modules.entries[k].type,
                                 frames[i].modules.entries[k].position);

    canvas_frame_wrap(canvas, frame);

    // create connectors
    if (i > 0) {
      Frame *previous_frame = allocator_frame_entry(
          canvas->frames[CanvasFrameState_Default].entries[i - 1]);
      canvas_connect_frames(canvas, previous_frame, frame);
    }
  }
}

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
  viewport_set_pan_sensitivity(60.0f);
  viewport_set_zoom_sensitivity(0.001f);
  unit_set_step(40.0f);

  module_manager_create_texture("textures/module_atlas.jpg",
                                TextureResolution_2048);

  Canvas canvas;
  create_frames(&canvas);

  Widget::CanvasShape canvas_shape = Widget::CanvasShape(gui, &canvas);
  canvas_shape.sync_shapes();
  renderer_add_draw_callback(renderer, canvas_draw_callback, &canvas_shape,
                             RendererDrawMode_All);

  renderer_draw(renderer);

  return 0;
}
