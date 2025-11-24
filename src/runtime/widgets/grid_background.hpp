#ifndef _GRID_BACKGROUND_HPP_
#define _GRID_BACKGROUND_HPP_

#include "nkengine/include/mesh.h"
#include "nkengine/include/texture.h"
#include "runtime/manager/viewport.h"
#include "webgpu/webgpu.h"
#include <imgui/imgui.h>

namespace Widget {

class GridBackground {

public:
  GridBackground() {}
  GridBackground(const char *, const TextureResolution);
  void draw();
  void draw_texture(WGPURenderPassEncoder);

  float grid_scale = 5.0f;
  float spacing = 32.0f;
  float dot_size = 3.0f;
  ImU32 bg_color = IM_COL32(20, 20, 20, 255);
  ImU32 dot_color = IM_COL32(120, 120, 120, 100);

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

} // namespace Widget

#endif
