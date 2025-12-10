#ifndef _WIDGET_CANVAS_SELECTION_H_
#define _WIDGET_CANVAS_SELECTION_H_

#include "runtime/node/canvas.h"
#include "runtime/node/connector_handle.h"
#include "runtime/widgets/canvas/core.hpp"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"

#include <imgui/imgui.h>

namespace Widget {
namespace Canvas {
class Selection : public Module {

public:
  Selection(Gui *gui, ::Canvas *node) : Module(gui, node) {
    gui_selection_init(&selection_connector);
    gui_highlight_init(&highlight);
  }

  typedef enum {
    State_None = 0,
    State_SelectConnectorHandle = 1 << 0,
    State_SelectNewConnectorHandle = 1 << 1,
    State_SelectConnector = 1 << 2,
    State_Deselect = 1 << 3,
    State_Freeze = 1 << 4,
  } State;

  void frame_selection_listen(Frame::Component *);
  void connector_selection_begin();
  void listen_connector_selection(Connector::Component *);
  void listen_connector_handle_selection(::Connector *);
  void connector_selection_end();

  const unsigned int get_state() { return state; }

  ::ConnectorHandle *active_connector_handle = nullptr;
  ::Connector *active_connector = nullptr;

  void listen_new_connector_handle(::Frame *, ::ConnectorHandle *,
                                   ConnectorHandle::Component *);

  void listen_active_connector_handle_release(::Connector *);

  void freeze() { flag_enable(State_Freeze, &state); };

  void unfreeze() { flag_disable(State_Freeze, &state); };

private:
  typedef enum {
    HighlightConfigurationType_Frame,
    HighlightConfigurationType_Module_N_Pod,
    HighlightConfigurationType_Connector,
    HighlightConfigurationType_COUNT,
  } HighlightConfigurationType;

  GuiHighlight highlight;
  GuiSelection selection_connector;
  unsigned int state = State_None;
};

} // namespace Canvas
}; // namespace Widget

#endif
