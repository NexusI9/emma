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
typedef void (*transform_box_on_session_start_callback)(void *);
typedef void (*transform_box_on_session_end_callback)(void *);

namespace Widget {

class TransformBox {

public:
  TransformBox(Gui *gui);

  static constexpr uint16_t OBJECT_CAPACITY = 16;

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

  typedef struct {
    void *handle;
    void *user_data;
    ImVec2 init_position;
    ImVec2 init_size;
    transform_box_set_position_callback set_position;
    transform_box_set_size_callback set_size;
    transform_box_get_position_callback get_position;
    transform_box_get_size_callback get_size;
    transform_box_on_session_start_callback session_start;
    transform_box_on_session_end_callback session_end;
  } Object;

  typedef struct {
    void *handle;
    transform_box_set_position_callback set_position;
    transform_box_set_size_callback set_size;
    transform_box_get_position_callback get_position;
    transform_box_get_size_callback get_size;
    transform_box_on_session_start_callback session_start;
    transform_box_on_session_end_callback session_end;
  } ObjectDescriptor;

  typedef struct {
    Object entries[OBJECT_CAPACITY];
    size_t count;
  } ObjectList;

  typedef enum {
    SessionStatus_Off,
    SessionStatus_BlankClick,
    SessionStatus_Hit,
  } SessionStatus;

  typedef enum {
    Mode_None = 0,
    Mode_Scale = 1 << 0,
    Mode_Move = 1 << 1,
    Mode_All = ~0,
  } Mode;

  Object *find_object(const void *, size_t *);
  Status toggle_object(const ObjectDescriptor *);
  StaticListStatus add_object(const ObjectDescriptor *);
  StaticListStatus remove_object(const void *, size_t *);
  StaticListStatus empty_objects();

  Status begin();
  Status end();

  Status update_bound_from_selection();
  Status update_bound(ImVec2, ImVec2);

  GuiSelection selection;

  uint16_t objects_count() { return objects.count; }

  unsigned int mode = Mode_All;
  ImGuiMouseButton button;
  void draw();

  static constexpr color primary_color = {87.0f / 255.0f, 154.0f / 255.0f,
                                          212.0f / 255.0f, 1.0f};
  static constexpr float area_padding = 20.0f;
  static constexpr uint8_t transform_box_handles_count = 9;
  static constexpr float transform_box_handle_size = 10.0f;
  static constexpr float stroke_width = 4.0f;

private:
  Gui *gui;
  ObjectList objects = {0};
  TransformHandle handles[transform_box_handles_count];

  // caches
  ImVec2 p0, p1, padded_area_0, padded_area_1;
  ImVec2 drag_start, drag_p0, drag_p1 = ImVec2(0, 0);

  // flags
  int active_handle = -1;

  // utils
  void handle_transform(const TransformHandleType, const ImVec2, const ImVec2,
                        const ImVec2, ImVec2 &, ImVec2 &);
  void transform_core(const TransformHandleType);
  void cache_initial_attributes();
  void clamp_mouse(const TransformHandleType, ImVec2 &);
};

} // namespace Widget

#endif
