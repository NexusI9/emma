#ifndef _WIDGET_SIDEBAR_CONTENT_MODULE_H_
#define _WIDGET_SIDEBAR_CONTENT_MODULE_H_

#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/module.h"
#include "runtime/manager/theme.h"
#include "runtime/widgets/core.hpp"
#include "runtime/widgets/sidebar/content/content.hpp"

namespace Widget {

namespace SideBar {

namespace Content {

namespace Modules {

class Component : public Content::Component {

public:
  Component(Gui *);

  void layout() override;
  void update() override;
  void render() override;

private:
  const WGPUTextureView module_view =
      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_Module);

  
  const float frame_rounding =
      gui_scale(gui, emma_size(ThemeEmmaSize_Width_Border_Large));
  
  const float cell_base_width = 90;
  const ImVec2 cell_size = ImVec2(gui_scale(gui, cell_base_width),
                                  gui_scale(gui, cell_base_width * 3 / 4));

  // cache size
  ImVec2 modules_size[ModuleType_COUNT];
  ImVec2 modules_position[ModuleType_COUNT];
  ImVec2 labels_position[ModuleType_COUNT];
  name_t labels[ModuleType_COUNT];
  ImVec2 frames_p0[ModuleType_COUNT];
  ImVec2 frames_p1[ModuleType_COUNT];
};

} // namespace Modules

} // namespace Content

} // namespace SideBar

} // namespace Widget

#endif
