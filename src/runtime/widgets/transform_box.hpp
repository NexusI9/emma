#ifndef _WIDGET_TRANSFORM_BOX_H_
#define _WIDGET_TRANSFORM_BOX_H_

#include "nkengine/include/gui.h"
#include "runtime/canvas/core.h"
#include "runtime/node/handle.h"
#include <cglm/cglm.h>
#include <imgui/imgui.h>

typedef void (*transform_box_set_position_callback)(void *, ImVec2);
typedef void (*transform_box_get_position_callback)(void *, ImVec2 &);

typedef void (*transform_box_set_size_callback)(void *, ImVec2);
typedef void (*transform_box_get_size_callback)(void *, ImVec2 &);

namespace Widget {

static constexpr uint16_t transform_box_objects_capacity = 16;

typedef enum {
  TransformBoxStatus_Success,
  TransformBoxStatus_MaxCapacity,
  TransformBoxStatus_OutOfBound,
  TransformBoxStatus_UndefError,
} TransformBoxStatus;

typedef struct {
  void *handle;
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

class TransformBox {

public:
  TransformBox(){};
  TransformBox(Gui *gui);

  TransformBoxObject *find_object(const void *, size_t *);
  TransformBoxStatus toggle_object(const TransformBoxObjectDescriptor *);
  StaticListStatus add_object(const TransformBoxObjectDescriptor *);
  StaticListStatus remove_object(const void *, size_t *);
  StaticListStatus empty_objects();

  TransformBoxStatus update_bound_from_selection();
  TransformBoxStatus update_bound(ImVec2, ImVec2);

  uint16_t objects_count() { return objects.count; }

  void draw();

private:
  ImVec2 p0, p1;
  Gui *gui;
  static constexpr uint8_t transform_box_handles_count = 9;
  static constexpr float transform_box_handle_size = 10.0f;

  Handle handles[transform_box_handles_count];
  int active_handle = -1;
  ImVec2 drag_start, drag_p0, drag_p1 = ImVec2(0, 0);

  TransformBoxObjectList objects = {0};

  void handle_transform(const HandleType, const ImVec2, const ImVec2,
                        const ImVec2, ImVec2 &, ImVec2 &);
  void transform_core(const HandleType);
  void clamp_mouse(const HandleType, ImVec2 &);
};

} // namespace Widget

#endif
