#ifndef _WIDGET_CANVAS_HPP_
#define _WIDGET_CANVAS_HPP_

#include "nkengine/include/gui.hpp"
#include "runtime/geometry/core.h"
#include "runtime/layouts/core.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"
#include "runtime/node/canvas/core.h"
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
#include "runtime/widgets/toolbar.hpp"
#include "runtime/widgets/transform/transform_box.hpp"
#include "utils/id.h"

namespace Widget {
namespace Canvas {
class Component : public Module, public Layout::Window {

public:
  Component(const char *, Gui *, ::Canvas *);

  void draw();
  void update_create_mode(const Create::Mode mode) {
    module.create.update_mode(mode);
  }

  typedef enum {
    State_None = 0,
    State_ShowOctagon = 1 << 0,
    State_ShowHeatmap = 1 << 1,
    State_FreezeSelection = 1 << 2,
    State_FreezeTransform = 1 << 3,
    State_FreezeCreationSession = 1 << 4,
  } State;

  // TODO maybe replace with more direct function like "freeze_selection()"
  void enable_state(const unsigned int state) {
    flag_enable(state, &this->state);

    if (State_FreezeSelection & state)
      module.selection.freeze();

    if (State_FreezeTransform & state)
      module.transform.freeze();

    if (State_FreezeCreationSession & state)
      module.create.freeze();
  }

  void disable_state(const unsigned int state) {
    flag_disable(state, &this->state);

    if (State_FreezeSelection & state)
      module.selection.unfreeze();

    if (State_FreezeTransform & state)
      module.transform.unfreeze();

    // if (State_FreezeCreation & state)
    // module.create.unfreeze();
  }

private:
  GridBackground::Component grid_background;
  ::Component::Sprite toolbar_glow;
  ImDrawList *dl;

  struct {
    Selection selection;
    Transform transform;
    Destroy destroy;
    Create create;
  } module;

  unsigned int state = State_None;

  void draw_selected_items_connector_handles();
  void draw_frame_handle_connectors(::Frame *, const int);
  void draw_frames();
  void draw_frames_octagon();
  void draw_pods();
  void draw_modules();
  void draw_connectors();

  bool disable_creation();
  bool disable_selection();
};

} // namespace Canvas
} // namespace Widget

#endif
