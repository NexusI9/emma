#ifndef _WIDGET_TRANSFORM_OBJECT_MANAGER_H_
#define _WIDGET_TRANSFORM_OBJECT_MANAGER_H_

#include "nkengine/include/list.h"
#include "runtime/node/transform_handle.h"
#include <imgui/imgui.h>

typedef void (*transform_box_set_position_callback)(void *, ImVec2);
typedef void (*transform_box_get_position_callback)(void *, ImVec2 &);
typedef void (*transform_box_set_size_callback)(void *, ImVec2);
typedef void (*transform_box_get_size_callback)(void *, ImVec2 &);
typedef void (*transform_box_on_selected_callback)(void *);
typedef void (*transform_box_on_session_start_callback)(void *);
typedef void (*transform_box_on_session_end_callback)(void *);

namespace Widget {
namespace Transform {

class ObjectManager {

public:
  static constexpr uint16_t OBJECT_CAPACITY = 16;

  typedef enum {
    Status_Success,
    Status_MaxCapacity,
    Status_ObjectAdded,
    Status_ObjectRemoved,
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

  Object *find_object(const void *, size_t *);
  Status toggle_object(const ObjectDescriptor *);
  StaticListStatus add_object(const ObjectDescriptor *);
  StaticListStatus remove_object(const void *, size_t *);
  StaticListStatus empty();
  uint16_t count() { return objects.count; }
  Object *get_entry(const size_t id) { return &objects.entries[id]; }

private:
  typedef struct {
    Object entries[OBJECT_CAPACITY];
    size_t count;
  } List;

  List objects = {0};
};
} // namespace Transform
} // namespace Widget

#endif
