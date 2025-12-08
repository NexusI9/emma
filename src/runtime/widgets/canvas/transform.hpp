#ifndef _WIDGET_CANVAS_TRANSFORM_H_
#define _WIDGET_CANVAS_TRANSFORM_H_

#include "runtime/node/canvas.h"
#include "runtime/widgets/canvas/core.hpp"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/transform/transform_box.hpp"
#include <imgui/imgui.h>

namespace Widget {

void canvas_shape_get_frame_position(void *, ImVec2 &);
void canvas_shape_get_frame_size(void *, ImVec2 &);

void canvas_shape_set_frame_position(void *, ImVec2);
void canvas_shape_set_frame_size(void *, ImVec2);

void canvas_shape_set_module_position(void *, ImVec2);
void canvas_shape_set_module_size(void *, ImVec2);

void canvas_shape_set_pod_position(void *, ImVec2);
void canvas_shape_set_pod_size(void *, ImVec2);

void canvas_shape_on_module_session_end(void *);

class CanvasTransform : public CanvasModule {

public:
  CanvasTransform(Gui *gui, Canvas *node);
  Transform::Box transform_box;

  typedef struct {
    Canvas *canvas;
    Frame *frame;
    FrameAllocList *parent_list;
  } FrameData;

  typedef enum {
    ConfigurationType_Frame,
    ConfigurationType_Module,
    ConfigurationType_Pod,
    ConfigurationType_COUNT,
  } ConfigurationType;

  typedef struct {
    // either scale, move or all, as instance for modules we only want to allow
    // move transformation.
    Transform::Box::Mode transform_mode;
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

  void listen_frame(FrameShape *, const ConfigurationType);

  void listen_active_connector_handle(ConnectorHandle *, Connector *);

  void freeze_transform() { flag_enable(State_Freeze, &state); }
  void unfreeze_transform() { flag_disable(State_Freeze, &state); }

  void begin() { transform_box.begin(); }
  void end() {
    if (transform_box.end() == Transform::Box::Status_ClearSelection) {
      canvas_empty_frame_state(node, CanvasFrameState_Selected);
      canvas_empty_module_state(node, CanvasModuleState_Selected);
      canvas_empty_pod_state(node, CanvasPodState_Selected);
    }
  }

private:
  typedef enum {
    State_None = 0,
    State_Freeze = 1 << 0,
  } State;

  struct {
    FrameData entries[ALLOCATOR_MAX_FRAMES];
    size_t count;
  } transform_frame_data;

  Configuration transform_configuration[ConfigurationType_COUNT] = {};

  Canvas *node;
  unsigned int state = State_None;
};

}; // namespace Widget

#endif
