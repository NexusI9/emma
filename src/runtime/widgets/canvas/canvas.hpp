#ifndef _WIDGET_CANVAS_HPP_
#define _WIDGET_CANVAS_HPP_

#include "runtime/geometry/core.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/node/canvas.h"
#include "runtime/node/connector.h"
#include "runtime/widgets/canvas/core.hpp"
#include "runtime/widgets/canvas/create.hpp"
#include "runtime/widgets/canvas/destroy.hpp"
#include "runtime/widgets/canvas/selection.hpp"
#include "runtime/widgets/canvas/transform.hpp"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"
#include "runtime/widgets/grid_background.hpp"
#include "runtime/widgets/tool_bar.hpp"
#include "runtime/widgets/transform/transform_box.hpp"
#include "utils/id.h"

namespace Widget {

class CanvasShape : public CanvasModule {

public:
  CanvasShape(Gui *, Canvas *);

  void draw();
  void update_create_mode(const CanvasCreate::Mode mode) {
    module.create.update_mode(mode);
  }

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
      module.selection.freeze_selection();
      module.transform.freeze_transform();
      break;

    default:
      break;
    }
  }
  void disable_state(const State state) {
    flag_disable(state, &this->state);

    switch (state) {

    case State_FreezeSelection:
      module.selection.unfreeze_selection();
      module.transform.unfreeze_transform();
      break;

    default:
      break;
    }
  }

private:
  GridBackground grid_background;
  ImDrawList *dl;

  struct {
    CanvasSelection selection;
    CanvasTransform transform;
    CanvasDestroy destroy;
    CanvasCreate create;
  } module;

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
