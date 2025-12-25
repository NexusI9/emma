#ifndef _WIDGET_CANVAS_CREATE_H_
#define _WIDGET_CANVAS_CREATE_H_

#include "runtime/manager/ui_sprite.h"
#include "runtime/node/canvas/core.h"
#include "runtime/node/connector_handle.h"
#include "runtime/widgets/canvas/core.hpp"
#include "runtime/widgets/connector.hpp"
#include "runtime/widgets/connector_handle.hpp"
#include "runtime/widgets/frame.hpp"

#include <imgui/imgui.h>

namespace Widget {
namespace Canvas {
class Create : public Module {

public:

  // Session states are reset at the end of each loop
  typedef enum {
    SessionState_None = 0,
    SessionState_Freeze = 1 << 0,
  } SessionState;

  typedef enum {
    Mode_Frame,
    Mode_Module,
    Mode_Pod,
    Mode_Note,
    Mode_Shape,
    Mode_COUNT,
  } Mode;

  Create(Gui *gui, ::Canvas *node) : Module(gui, node) {
    glm_vec2_copy((float *)ui_sprite(UISprite_Pod_Base)->size, pod_half_size);
    glm_vec2_scale(pod_half_size, 0.5f, pod_half_size);
  }
  void update_mode(const Mode mode) { this->mode = mode; }
  
  void begin();
  void end();

  void freeze() { flag_enable(SessionState_Freeze, &state); };
  void unfreeze() { flag_disable(SessionState_Freeze, &state); };
 

private:
  Mode mode = Mode_Frame;
  unsigned int state = SessionState_None;

  vec2 pod_half_size;
  ::Frame *new_frame = nullptr;

  void get_mouse_position(vec2);

  void frame_create();
  void frame_resize();
  void frame_release();
  void module();
  void pod();
  void shape();
  void note();
};

} // namespace Canvas
} // namespace Widget

#endif
