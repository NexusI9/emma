#ifndef _WIDGET_CANVAS_TRANSFORM_H_
#define _WIDGET_CANVAS_TRANSFORM_H_

#include "runtime/manager/allocator.h"
#include "runtime/node/canvas/state.h"
#include "runtime/widgets/canvas/core.hpp"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/transform/transform_box.hpp"
#include <imgui/imgui.h>

namespace Widget {

namespace Canvas {

void canvas_shape_get_frame_position(void *, ImVec2 &);
void canvas_shape_get_frame_size(void *, ImVec2 &);

void canvas_shape_set_frame_position(void *, ImVec2);
void canvas_shape_set_frame_size(void *, ImVec2);

void canvas_shape_set_module_position(void *, ImVec2);
void canvas_shape_set_module_size(void *, ImVec2);

void canvas_shape_set_pod_position(void *, ImVec2);
void canvas_shape_set_pod_size(void *, ImVec2);

void canvas_shape_on_module_session_end(void *);

class Transform : public Module {

public:
  Transform(Gui *gui, ::Canvas *node);
  ::Widget::Transform::Box transform_box;

  typedef struct {
    ::Canvas *canvas;
    ::Frame *frame;
    FrameAllocList *parent_list;
  } FrameData;

  typedef enum {
    ConfigurationType_Frame,
    ConfigurationType_Module,
    ConfigurationType_Pod,
    ConfigurationType_COUNT,
  } ConfigurationType;

  typedef enum {
    State_None = 0,
    State_Freeze = 1 << 0,
    State_Dragging = 1 << 1,
    State_Deselect = 1 << 2,
  } State;

  typedef struct {
    // either scale, move or all, as instance for modules we only want to allow
    // move transformation.
    ::Widget::Transform::Box::Mode transform_mode;
    // Size accessor of the object
    transform_box_get_size_callback get_size;
    // Position accessor of the object
    transform_box_get_position_callback get_position;
    // Size mutator of the object in world space, should handle boundbox resize
    // as well.
    transform_box_set_size_callback set_size;
    // Position accessor of the object in world space,should handle boundbox
    // resize as well.
    transform_box_set_position_callback set_position;
    // What to check after an object is done being transformed. Can be useful to
    // handle parent/child hierarchy.
    transform_box_on_session_end_callback session_end;
    // Target list to register the frames as "selected" and update the UI
    // accordingly (outlines, show connector handles...)
    FrameAllocList *selection_list;
    // Use in the transform callback on session end to check if a module is
    // colliding with a parent to add it as a child or at the opposite, going
    // out of a parent and break the relationship.
    FrameAllocList *parent_list;
    // The button which triggers the transformation (ImGuiMouseButton_Right...)
    ImGuiMouseButton button;
  } Configuration;

  void listen_frame(Frame::Component *, const ConfigurationType);

  const unsigned int get_state() { return state; }

  void listen_active_connector_handle(::ConnectorHandle *, ::Connector *);

  void freeze() { flag_enable(State_Freeze, &state); }
  void unfreeze() { flag_disable(State_Freeze, &state); }

  void begin() { transform_box.begin(); }
  void end() {
    if (transform_box.end() ==
        ::Widget::Transform::Box::Status_ClearSelection) {
      flag_enable(State_Deselect, &state);
      canvas_empty_frame_state(node, CanvasFrameState_Selected);
      canvas_empty_module_state(node, CanvasModuleState_Selected);
      canvas_empty_pod_state(node, CanvasPodState_Selected);
    } else {
      flag_disable(State_Deselect, &state);
    }
  }

private:
  struct {
    FrameData entries[ALLOCATOR_FRAME_CAPACITY];
    size_t count;
  } transform_frame_data;

  Configuration transform_configuration[ConfigurationType_COUNT] = {};

  unsigned int state = State_None;
};

} // namespace Canvas
}; // namespace Widget

#endif
