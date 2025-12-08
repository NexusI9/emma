#ifndef _WIDGET_CANVAS_HPP_
#define _WIDGET_CANVAS_HPP_

#include "runtime/geometry/core.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/node/canvas.h"
#include "runtime/node/connector.h"
#include "runtime/widgets/canvas/selection.hpp"
#include "runtime/widgets/canvas/transform.hpp"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/grid_background.hpp"
#include "runtime/widgets/tool_bar.hpp"
#include "runtime/widgets/transform_box.hpp"
#include "utils/id.h"

namespace Widget {

typedef enum {
  CanvasCreateMode_Frame,
  CanvasCreateMode_Module,
  CanvasCreateMode_Pod,
  CanvasCreateMode_Note,
  CanvasCreateMode_Shape,
  CanvasCreateMode_COUNT,
} CanvasCreateMode;

class CanvasShape {

public:
  CanvasShape(Gui *, Canvas *);

  void draw();
  void update_create_mode(const CanvasCreateMode mode) { create_mode = mode; }

  typedef enum {
    State_None = 0,
    State_ShowOctagon = 1 << 0,
    State_FreezeSelection = 1 << 1,
  } State;

  // TODO maybe replace with more direct function like "freeze_selection()"
  void enable_state(const State state) {

    flag_enable(state, &this->state);

    switch (state) {

    case State_FreezeSelection:
      canvas_selection.freeze_selection();
      canvas_transform.freeze_transform();
      break;

    default:
      break;
    }
  }
  void disable_state(const State state) {
    flag_disable(state, &this->state);

    switch (state) {

    case State_FreezeSelection:
      canvas_selection.unfreeze_selection();
      canvas_transform.unfreeze_transform();
      break;

    default:
      break;
    }
  }

private:
  Gui *gui;
  Canvas *node;
  GridBackground grid_background;
  ImDrawList *dl;

  CanvasSelection canvas_selection;
  CanvasTransform canvas_transform;

  CanvasCreateMode create_mode = CanvasCreateMode_Frame;

  unsigned int state = State_None;

  void draw_selected_items_connector_handles();
  void draw_frame_handle_connectors(Frame *, const int);
  void draw_frames();
  void draw_frames_octagon();
  void draw_pods();
  void draw_modules();
  void draw_connectors();
};

} // namespace Widget

#endif
