#ifndef _GRID_BACKGROUND_HPP_
#define _GRID_BACKGROUND_HPP_

#include "nkengine/include/mesh.h"
#include "nkengine/include/texture.h"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/viewport.h"
#include "runtime/widgets/utils.hpp"
#include "webgpu/webgpu.h"
#include <imgui/imgui.h>

namespace Widget {

namespace GridBackground {
class Component {

public:
  Component(const char *, const TextureResolution);
  void draw();
  void draw_texture(WGPURenderPassEncoder);

  float grid_scale = 5.0f;
  float spacing = 32.0f;
  float dot_size = 3.0f;
  const float *bg_color =
      emma_color(ThemeEmmaColor_Background_System_Strong_On_Dark);
  const ImColor dot_color = emma_im_color(ThemeEmmaColor_Icon_Subtlest_On_Dark);

private:
  void create_grid_texture(const char *, const TextureResolution);
  void create_mesh();
  void update_uv();

  TextureResolution texture_resolution;
  WGPUTexture grid_texture;
  WGPUTextureView grid_view;
  WGPUBuffer texture_coordinates_buffer;
  TextureCoordinatesUniform texture_coo_uniform;
  vec2 base_tile;

  Mesh *quad;
  MeshDrawPacket draw_packet;
};

} // namespace GridBackground
} // namespace Widget

#endif
