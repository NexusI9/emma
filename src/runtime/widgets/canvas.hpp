#ifndef _WIDGET_CANVAS_HPP_
#define _WIDGET_CANVAS_HPP_

#include "runtime/geometry/core.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/node/canvas.h"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/grid_background.hpp"
#include "runtime/widgets/tool_bar.hpp"
#include "runtime/widgets/transform_box.hpp"

namespace Widget {

void canvas_shape_set_frame_shape_position(void *, ImVec2);
void canvas_shape_get_frame_shape_position(void *, ImVec2 &);
void canvas_shape_set_frame_shape_size(void *, ImVec2);
void canvas_shape_get_frame_shape_size(void *, ImVec2 &);
void canvas_shape_on_frame_selection(void *);
void canvas_shape_on_module_session_end(void *);

typedef struct {
  Canvas *canvas;
  Frame *frame;
  FrameAllocList *parent_list;
} CanvasTransformFrameData;

typedef struct {
  // either scale, move or all, as instance for modules we only want to allow
  // move transformation.
  TransformBoxMode transform_mode;
  // Size accessor of the object
  transform_box_get_size_callback get_size;
  // Position accessor of the object
  transform_box_get_position_callback get_position;
  // Size mutator of the object in world space, should handle boundbox resize as
  // well.
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
  // colliding with a parent to add it as a child or at the opposite, going out
  // of a parent and break the relationship.
  FrameAllocList *parent_list;
} CanvasTransformConfiguration;

typedef enum {
  CanvasTransformConfigurationType_Frame,
  CanvasTransformConfigurationType_Module,
  CanvasTransformConfigurationType_COUNT,
} CanvasTransformConfigurationType;

class CanvasShape {

public:
  CanvasShape(Gui *, Canvas *);

  void draw(bool);

private:
  Gui *gui;
  Canvas *node;
  TransformBox transform_box;
  GridBackground grid_background;

  struct {
    CanvasTransformFrameData entries[ALLOCATOR_MAX_FRAMES];
    size_t count;
  } transform_frame_data;

  CanvasTransformConfiguration
      transform_configuration[CanvasTransformConfigurationType_COUNT] = {};

  void draw_frame_transform_begin(Frame *,
                                  const CanvasTransformConfiguration *);

  void draw_frame_handle_connectors(Frame *, const int);
}; // namespace Widget

} // namespace Widget

#endif
