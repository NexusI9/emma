#ifndef _WIDGET_CANVAS_INTERACTION_H_
#define _WIDGET_CANVAS_INTERACTION_H_

#include "runtime/node/canvas.h"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"

#include <imgui/imgui.h>

namespace Widget {

class CanvasInteraction {

public:
  CanvasInteraction(Canvas *node) : node(node) {
    gui_selection_init(&selection_connector);
    gui_highlight_init(&highlight);
  }

  void frame_highlight_listen(FrameShape *);

  void connector_highlight_begin();
  void listen_connector_highlight(ConnectorShape *);
  void listen_connector_handle_highlight(Connector *);
  void connector_highlight_end();

  void listen_new_connector_handle(Frame *, ConnectorHandle *,
                                   ConnectorHandleShape *);

  void listen_active_connector_handle_transform(Connector *);
  void listen_active_connector_handle_release(Connector *);

  CanvasStatus destroy_listen();

  void freeze_selection() {
    flag_enable(SelectionState_Freeze, &selection_state);
  };

  void unfreeze_selection() {
    flag_disable(SelectionState_Freeze, &selection_state);
  };

private:
  Canvas *node;
  ConnectorHandle *active_connector_handle = nullptr;
  Connector *active_connector = nullptr;

  typedef enum {
    SelectionState_None = 0,
    SelectionState_ConnectorHandle = 1 << 0,
    SelectionState_NewConnectorHandle = 1 << 1,
    SelectionState_Freeze = 1 << 2,
  } SelectionState;

  typedef enum {
    HighlightConfigurationType_Frame,
    HighlightConfigurationType_Module_N_Pod,
    HighlightConfigurationType_Connector,
    HighlightConfigurationType_COUNT,
  } HighlightConfigurationType;

  GuiHighlight highlight;
  GuiSelection selection_connector;
  unsigned int selection_state = SelectionState_None;
};

}; // namespace Widget

#endif
