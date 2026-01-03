#ifndef _WIDGET_SIDEBAR_CONTENT_MODULE_EDITOR_H_
#define _WIDGET_SIDEBAR_CONTENT_MODULE_EDITOR_H_

#include "runtime/widgets/sidebar/content/content.hpp"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Modules {

namespace Editor {

class Component : public Content::Component {

public:
  Component(Gui *);

  void layout() override;
  void draw() override;
};

} // namespace Editor

} // namespace Modules

} // namespace Content

} // namespace SideBar

} // namespace Widget

#endif
