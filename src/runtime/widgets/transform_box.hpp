#ifndef _WIDGET_TRANSFORM_BOX_H_
#define _WIDGET_TRANSFORM_BOX_H_

#include "nkengine/include/gui.h"
#include "runtime/node/transform_handle.h"
#include "runtime/widgets/transform_handle.hpp"
#include <cglm/cglm.h>
#include <imgui/imgui.h>

typedef void (*transform_box_set_position_callback)(void *, ImVec2);
typedef void (*transform_box_get_position_callback)(void *, ImVec2 &);
typedef void (*transform_box_set_size_callback)(void *, ImVec2);
typedef void (*transform_box_get_size_callback)(void *, ImVec2 &);
typedef void (*transform_box_on_selected_callback)(void *);

namespace Widget {

static constexpr uint16_t transform_box_objects_capacity = 16;

typedef enum {
  TransformBoxStatus_Success,
  TransformBoxStatus_MaxCapacity,
  TransformBoxStatus_ObjectAdded,
  TransformBoxStatus_ObjectRemoved,
  TransformBoxStatus_SessionAlreadyStarted,
  TransformBoxStatus_ClearSelection,
  TransformBoxStatus_OutOfBound,
  TransformBoxStatus_UndefError,
} TransformBoxStatus;

typedef struct {
  void *handle;
  void *user_data;
  ImVec2 init_position;
  ImVec2 init_size;
  transform_box_set_position_callback set_position;
  transform_box_set_size_callback set_size;
  transform_box_get_position_callback get_position;
  transform_box_get_size_callback get_size;
} TransformBoxObject;

typedef struct {
  void *handle;
  transform_box_set_position_callback set_position;
  transform_box_set_size_callback set_size;
  transform_box_get_position_callback get_position;
  transform_box_get_size_callback get_size;
} TransformBoxObjectDescriptor;

typedef struct {
  TransformBoxObject entries[transform_box_objects_capacity];
  size_t count;
} TransformBoxObjectList;

typedef enum {
  TransformBoxSessionStatus_Off,
  TransformBoxSessionStatus_BlankClick,
  TransformBoxSessionStatus_Hit,
} TransformBoxSessionStatus;

class TransformBox {

public:
  TransformBox(Gui *gui);

  TransformBoxObject *find_object(const void *, size_t *);
  TransformBoxStatus toggle_object(const TransformBoxObjectDescriptor *);
  StaticListStatus add_object(const TransformBoxObjectDescriptor *);
  StaticListStatus remove_object(const void *, size_t *);
  StaticListStatus empty_objects();

  TransformBoxStatus update_bound_from_selection();
  TransformBoxStatus update_bound(ImVec2, ImVec2);

  TransformBoxStatus session_set_blank_click();
  TransformBoxStatus session_set_hit();
  TransformBoxStatus session_end();

  uint16_t objects_count() { return objects.count; }

  void draw();

private:
  static constexpr float area_padding = 20.0f;
  static constexpr uint8_t transform_box_handles_count = 9;
  static constexpr float transform_box_handle_size = 10.0f;
  static constexpr color primary_color = {87.0f / 255.0f, 154.0f / 255.0f,
                                          212.0f / 255.0f, 1.0f};

  static constexpr float stroke_width = 4.0f;

  Gui *gui;
  TransformBoxObjectList objects = {0};
  TransformHandle handles[transform_box_handles_count];

  // caches
  ImVec2 p0, p1, padded_area_0, padded_area_1;
  ImVec2 drag_start, drag_p0, drag_p1 = ImVec2(0, 0);

  // flags
  int active_handle = -1;
  TransformBoxSessionStatus session_status = TransformBoxSessionStatus_Off;

  // utils
  void handle_transform(const TransformHandleType, const ImVec2, const ImVec2,
                        const ImVec2, ImVec2 &, ImVec2 &);
  void transform_core(const TransformHandleType);
  void cache_initial_attributes();
  void clamp_mouse(const TransformHandleType, ImVec2 &);
};

} // namespace Widget

#endif
