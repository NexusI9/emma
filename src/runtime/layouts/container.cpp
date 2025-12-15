#include "container.hpp"
#include "resources/theme.emma.h"
#include "runtime/geometry/boundbox.h"
#include "runtime/geometry/core.h"
#include "runtime/layouts/navbar.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/unit.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/canvas.h"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/canvas/canvas.hpp"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/heatmap.hpp"
#include "runtime/widgets/utils.hpp"
#include "utils/input.h"

Layout::Container::Component::Component(
    Gui *gui, Canvas *canvas,
    Heatmap *hm[Widget::Heatmap::Selector::Type_COUNT])
    : Core(gui, canvas), canvas("Canvas Window", gui, canvas),
      toolbar("Toolbar Window", gui,
              texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI)),
      heatmap_selector("Heatmap Window", gui, hm),
      sidebar("Sidebar Window", gui, canvas),
      navbar("Navbar Window", gui, canvas,
             {
                 .setter = set_octalysis_state,
                 .getter = get_octalysis_state,
                 .init_state = display_state_enabled(DisplayState_Octagon),
                 .user_data = this,
             },
             {
                 .setter = set_heatmap_state,
                 .getter = get_heatmap_state,
                 .init_state = display_state_enabled(DisplayState_Heatmap),
                 .user_data = this,
             }) {

  heatmap_selector.disable();

  // Setup Interactions callbacks
  toolbar.add_callback(on_toolbar_update, this);
  sidebar.add_tab_update_callback(on_sidebar_tab_update, this);
  sidebar.content.modules.add_drag_begin_callback(on_module_drag_begin, this);
  sidebar.content.modules.add_drag_callback(on_module_drag, this);
  sidebar.content.modules.add_drag_end_callback(on_module_drag_end, this);
}

void Layout::Container::Component::draw() {

  gui_command_begin(gui);
  gui_draw_update_io(gui);

  if (display_state_enabled(DisplayState_Heatmap))
    heatmap_selector.listen_update();

  gui_draw_swapchain_begin(gui);
  ImGui_ImplWGPU_NewFrame();
  ImGui::NewFrame();

  /*
    The window draw call order is front first because we use ImGuiWindowFlags
    that prevent putting the focused window in front of the others (this ensure
    static and fully controled window z-indexing).
    However it seems like ImGui maintain the first window call in front of the
    other, meaning the next calls will appear behind the first called window.
   */
  {
    navbar.draw();

    heatmap_selector.draw();

    sidebar.draw();
    toolbar.draw();
    canvas.draw();
  }

  ImGui::Render();
  ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), gui->pass_encoder);

  gui_draw_swapchain_end(gui);
  gui_command_end(gui);

  viewport_update(&g_viewport_manager);
}

void Layout::Container::draw_callback(Renderer *renderer, void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;
  container->draw();
}

void Layout::Container::set_octalysis_state(bool value, void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;

  if (value) {
    container->enable_display_state(
        Layout::Container::Component::DisplayState_Octagon);
  } else {
    container->disable_display_state(
        Layout::Container::Component::DisplayState_Octagon);
  }
}

void Layout::Container::set_heatmap_state(bool value, void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;

  if (value) {
    container->enable_display_state(
        Layout::Container::Component::DisplayState_Heatmap);
  } else {
    container->disable_display_state(
        Layout::Container::Component::DisplayState_Heatmap);
  }

  container->heatmap_selector.require_update();
}

bool Layout::Container::get_octalysis_state(void *data) {
  return ((Layout::Container::Component *)data)
      ->display_state_enabled(
          Layout::Container::Component::DisplayState_Octagon);
}

bool Layout::Container::get_heatmap_state(void *data) {
  return ((Layout::Container::Component *)data)
      ->display_state_enabled(
          Layout::Container::Component::DisplayState_Heatmap);
}

void Layout::Container::on_toolbar_update(const uint8_t active, void *data) {
  Layout::Container::Component *container =
      (Layout::Container::Component *)data;

  container->canvas.update_create_mode((Widget::Canvas::Create::Mode)active);
  container->canvas.enable_state(
      Widget::Canvas::Component::State::State_FreezeCreationSession);
}

void Layout::Container::on_sidebar_tab_update(const uint8_t active,
                                              void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;

  container->canvas.enable_state(
      Widget::Canvas::Component::State::State_FreezeCreationSession);
}

void Layout::Container::on_module_drag_begin(const TextureAtlasRegion *sprite,
                                             const ModuleType type,
                                             void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;

  container->canvas.enable_state(
      Widget::Canvas::Component::State::State_FreezeCreationSession);

  container->canvas.enable_state(
      Widget::Canvas::Component::State::State_FreezeSelection);
}

/**
   On module release: Add the module to the targeted frame (if any) and enable
   back Selection and Transform states
 */
void Layout::Container::on_module_drag_end(const TextureAtlasRegion *sprite,
                                           const ModuleType type,
                                           const ImVec2 position, void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;

  ::Canvas *canvas = container->canvas.get_node();

  vec2 pos = {position.x, position.y};

  // TODO: Frustrum frame in viewport for faster match
  bool hit_frame = false;
  for (size_t i = 0; i < canvas->frames->count; i++) {

    Frame *frame = allocator_frame_entry(canvas->frames->entries[i]);

    if (boundbox_contain_point(&frame->boundbox, pos)) {

      // convert sprite scrren space to frame local space and center it
      vec2 local_pos = {
          unit_snap(pos[0] - frame->world_position[0] - sprite->size[0] / 2),
          unit_snap(pos[1] - frame->world_position[1] - sprite->size[1] / 2),
      };

      canvas_add_module_to_frame(canvas, frame, type, local_pos);
      hit_frame = true;
      break;
    }
  }

  if (!hit_frame) {
    Frame *module = canvas_create_module(canvas, type);
    if (module)
      canvas_set_module_world_position(
          canvas, module,
          (vec2){
              unit_snap(pos[0] - sprite->size[0] / 2),
              unit_snap(pos[1] - sprite->size[1] / 2),
          });
  }

  container->canvas.enable_state(
      Widget::Canvas::Component::State::State_FreezeCreationSession);

  container->canvas.disable_state(
      Widget::Canvas::Component::State::State_FreezeSelection);
}

/**
   Check if the sprite hover one of the frame and highlight it according to
   signal the module will be added to the hovered frame.
 */
void Layout::Container::on_module_drag(const TextureAtlasRegion *sprite,
                                       const ModuleType type,
                                       const ImVec2 position, void *data) {

  Layout::Container::Component *container =
      (Layout::Container::Component *)data;

  ::Canvas *canvas = container->canvas.get_node();

  // TODO: Frustrum frame
  for (size_t i = 0; i < canvas->frames->count; i++) {

    Frame *frame = allocator_frame_entry(canvas->frames->entries[i]);

    RectCoordinate vp_boundbox;
    vp2(frame->boundbox.p0, vp_boundbox.p0);
    vp2(frame->boundbox.p1, vp_boundbox.p1);

    if (boundbox_contain_point(&frame->boundbox,
                               (vec2){position.x, position.y})) {
      Widget::Frame::Component(frame).draw_highlight();
      break;
    }
  }
}
