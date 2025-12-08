#ifndef _WIDGET_CANVAS_DESTROY_H_
#define _WIDGET_CANVAS_DESTROY_H_

#include "runtime/node/canvas.h"
#include "runtime/node/connector_handle.h"
#include "runtime/widgets/canvas/core.hpp"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"

#include <imgui/imgui.h>

namespace Widget {

class CanvasDestroy : public CanvasModule {

public:
  CanvasDestroy(Gui *gui, Canvas *node) : CanvasModule(gui, node) {}

  bool begin() { return input_key(INPUT_KEY_BACKSPACE); }
  void active_connector(Connector **);
  CanvasStatus selected_frames();
};

} // namespace Widget

#endif
