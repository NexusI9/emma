#ifndef _LAYOUT_CORE_H_
#define _LAYOUT_CORE_H_

#include "runtime/node/canvas.h"

namespace Layout {

class Core {

public:
  Core(Gui *gui, Canvas *canvas) : gui(gui), canvas(canvas) {}

protected:
  Gui *gui;
  Canvas *canvas;
};

} // namespace Layout

#endif
