#ifndef _WIDGET_SIDEBAR_CONTENT_PERSONAS_H_
#define _WIDGET_SIDEBAR_CONTENT_PERSONAS_H_

#include "runtime/widgets/sidebar/content/content.hpp"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Personas {

class Component : public Content::Component {

public:
  Component(Gui *);

  void layout() override;
  void draw() override;

private:
  
};
  
} // namespace Personas
} // namespace Content
} // namespace SideBar
} // namespace Widget

#endif
