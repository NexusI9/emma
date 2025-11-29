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

typedef struct {
  Canvas *canvas;
  Frame *frame;
} CanvasTransformFrameData;

typedef struct {
  TransformBoxMode transform_mode;
  transform_box_get_size_callback get_size;
  transform_box_get_position_callback get_position;
  transform_box_set_size_callback set_size;
  transform_box_set_position_callback set_position;
  AllocIdRefList selection_list;
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

  void draw_frame_transform(Frame *, const CanvasTransformConfiguration *);

  void draw_frame_handle_connectors(Frame *, const int);
}; // namespace Widget

} // namespace Widget

#endif
