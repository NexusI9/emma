#ifndef _WIDGET_TRANSFORM_BOX_H_
#define _WIDGET_TRANSFORM_BOX_H_

#include "nkengine/include/gui.h"
#include "runtime/node/transform_handle.h"
#include "runtime/widgets/transform/handle.hpp"
#include "runtime/widgets/transform/object_manager.hpp"
#include <cglm/cglm.h>
#include <imgui/imgui.h>

namespace Widget {

namespace Transform {
class Box {

public:
  Box(Gui *gui);

  static constexpr float area_padding = 20.0f;
  static constexpr uint8_t transform_box_handles_count = 9;
  static constexpr float transform_box_handle_size = 10.0f;
  static constexpr float stroke_width = 4.0f;

  typedef enum {
    Status_Success,
    Status_MaxCapacity,
    Status_ObjectAdded,
    Status_ObjectRemoved,
    Status_SessionAlreadyStarted,
    Status_ClearSelection,
    Status_OutOfBound,
    Status_UndefError,
  } Status;

  typedef enum {
    State_None = 0,
    State_Dragging = 1 << 0,
    State_Shown = 1 << 1,
  } State;

  typedef enum {
    Mode_None = 0,
    Mode_Scale = 1 << 0,
    Mode_Move = 1 << 1,
    Mode_All = ~0,
  } Mode;

  Status begin();
  Status end();

  Status update_bound_from_selection();
  Status update_bound(ImVec2, ImVec2);

  GuiSelection selection;

  ObjectManager::Object *find_object(const void *handle, size_t *id) {
    return object_manager.find_object(handle, id);
  }

  ObjectManager::Status
  toggle_object(const ObjectManager::ObjectDescriptor *desc) {
    ObjectManager::Status result = object_manager.toggle_object(desc);
    update_visibility_state();

    return result;
  }

  StaticListStatus add_object(const ObjectManager::ObjectDescriptor *desc) {
    StaticListStatus result = object_manager.add_object(desc);
    update_visibility_state();

    return result;
  }

  StaticListStatus remove_object(const void *handle, size_t *id) {
    StaticListStatus result = object_manager.remove_object(handle, id);
    update_visibility_state();

    return result;
  }

  StaticListStatus empty() {
    StaticListStatus result = object_manager.empty();
    update_visibility_state();

    return result;
  }
  uint16_t objects_count() { return object_manager.count(); }

  unsigned int get_state() { return state; }
  unsigned int mode = Mode_All;

  ImGuiMouseButton button;
  void draw();

private:
  Gui *gui;
  TransformHandle handles[transform_box_handles_count];
  ObjectManager object_manager;

  // caches
  ImVec2 p0, p1, padded_area_0, padded_area_1;
  ImVec2 drag_start, drag_p0, drag_p1 = ImVec2(0, 0);

  // flags
  int active_handle = -1;
  unsigned int state = State_None;

  // utils
  void update_visibility_state();
  void handle_transform(const TransformHandleType, const ImVec2, const ImVec2,
                        const ImVec2, ImVec2 &, ImVec2 &);
  void transform_core(const TransformHandleType);
  void cache_initial_attributes();
  void clamp_mouse(const TransformHandleType, ImVec2 &);
};

} // namespace Transform
} // namespace Widget

#endif
