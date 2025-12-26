#ifndef _WIDGET_SIDEBAR_CONTENT_MODULE_H_
#define _WIDGET_SIDEBAR_CONTENT_MODULE_H_

#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/module.h"
#include "runtime/manager/theme.h"
#include "runtime/node/frame.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/sidebar/content/content.hpp"
#include "utils/callback.h"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Modules {

typedef void (*on_drag_begin)(const TextureAtlasRegion *, const ModuleType,
                              void *);

typedef void (*on_drag)(const TextureAtlasRegion *, const ModuleType,
                        const ImVec2, void *);

typedef void (*on_drag_end)(const TextureAtlasRegion *, const ModuleType,
                            const ImVec2, void *);

typedef void (*on_module_click)(const ModuleType, bool, void *);

typedef CALLBACK_ENTRY(on_drag_begin) OnDragBeginCallback;
typedef CALLBACK_ENTRY(on_drag) OnDragCallback;
typedef CALLBACK_ENTRY(on_drag_end) OnDragEndCallback;
typedef CALLBACK_ENTRY(on_module_click) OnModuleClickCallback;

class Component : public Content::Component {

public:
  Component(Gui *);

  void layout() override;
  void draw() override;

  void clear_selection() {
    active_thumbnail = nullptr;
    selected_thumbnail = -1;
  }

  StaticListStatus add_drag_begin_callback(on_drag_begin cb, void *data) {
    OnDragBeginCallback entry = {cb, data};
    return stli_insert(drag_begin_callbacks.entries, DRAG_CALLBACK_CAPACITY,
                       &drag_begin_callbacks.count, sizeof(OnDragBeginCallback),
                       &entry, "Drag Start Callback");
  }

  StaticListStatus add_drag_callback(on_drag_end cb, void *data) {
    OnDragEndCallback entry = {cb, data};
    return stli_insert(drag_callbacks.entries, DRAG_CALLBACK_CAPACITY,
                       &drag_callbacks.count, sizeof(OnDragCallback), &entry,
                       "Drag Callback");
  }

  StaticListStatus add_drag_end_callback(on_drag_end cb, void *data) {
    OnDragEndCallback entry = {cb, data};
    return stli_insert(drag_end_callbacks.entries, DRAG_CALLBACK_CAPACITY,
                       &drag_end_callbacks.count, sizeof(OnDragEndCallback),
                       &entry, "Drag End Callback");
  }

  StaticListStatus add_module_click_callback(on_module_click cb, void *data) {
    OnModuleClickCallback entry = {cb, data};
    return stli_insert(module_click_callbacks.entries, DRAG_CALLBACK_CAPACITY,
                       &module_click_callbacks.count,
                       sizeof(OnModuleClickCallback), &entry,
                       "Module Click Callback");
  }

private:
  const WGPUTextureView module_view =
      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_Module);

  const float frame_rounding =
      gui_scale(gui, emma_size(ThemeEmmaSize_Radius_Base));

  const float thickness =
      gui_scale(gui, emma_size(ThemeEmmaSize_Width_Border_Base));

  const float cell_base_width = 90;
  const ImVec2 cell_size = ImVec2(gui_scale(gui, cell_base_width),
                                  gui_scale(gui, cell_base_width * 3 / 4));

  // cache coordinates
  // TODO: Convert those to UINode
  // clang-format off
  ImVec2 modules_size[ModuleType_COUNT],
         modules_position[ModuleType_COUNT],

         frames_p0[ModuleType_COUNT],
         frames_p1[ModuleType_COUNT],

         labels_position[ModuleType_COUNT];
  // clang-format on

  name_t labels[ModuleType_COUNT];

  static constexpr uint8_t DRAG_CALLBACK_CAPACITY = 32;
  STATIC_LIST(OnDragBeginCallback, DRAG_CALLBACK_CAPACITY) drag_begin_callbacks;
  STATIC_LIST(OnDragEndCallback, DRAG_CALLBACK_CAPACITY) drag_callbacks;
  STATIC_LIST(OnDragEndCallback, DRAG_CALLBACK_CAPACITY) drag_end_callbacks;
  STATIC_LIST(OnModuleClickCallback, DRAG_CALLBACK_CAPACITY)
  module_click_callbacks;

  // Drag data
  const TextureAtlasRegion *active_thumbnail = nullptr;
  int32_t selected_thumbnail = -1;
  ModuleType thumbnail_index;
  ImVec2 mouse_init_pos;

  void drag_module_begin(const ModuleType);
  void drag_module();
  void drag_module_end();
};

} // namespace Modules

} // namespace Content

} // namespace SideBar

} // namespace Widget

#endif
