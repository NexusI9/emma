#include "editor.hpp"

Widget::SideBar::Content::Modules::Editor::Component::Component(Gui *gui)
    : Content::Component(gui) {}

void Widget::SideBar::Content::Modules::Editor::Component::layout() {}

void Widget::SideBar::Content::Modules::Editor::Component::draw() {

  draw_header("Edit Module");
}
