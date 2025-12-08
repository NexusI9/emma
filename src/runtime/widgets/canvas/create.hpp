#ifndef _WIDGET_CANVAS_CREATE_H_
#define _WIDGET_CANVAS_CREATE_H_

#include "runtime/node/canvas.h"
#include "runtime/node/connector_handle.h"
#include "runtime/widgets/canvas/core.hpp"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"

#include <imgui/imgui.h>

namespace Widget {

class CanvasCreate : public CanvasModule {

public:
  CanvasCreate(Gui *gui, Canvas *node) : CanvasModule(gui, node) {}


};

} // namespace Widget

#endif
