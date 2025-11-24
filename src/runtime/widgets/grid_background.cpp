#include "grid_background.hpp"
#include "imgui/imgui_impl_wgpu.h"
#include "nkengine/include/context.h"
#include "nkengine/include/resource_manager.h"
#include "nkengine/include/texture.h"
#include "runtime/manager/viewport.h"
#include "webgpu/webgpu.h"
#include <cglm/cglm.h>

Widget::GridBackground::GridBackground(const char *path,
                                       const TextureResolution resolution) {

  // create texture
  create_grid_texture(path, resolution);
  create_mesh();
}

void Widget::GridBackground::update_uv() {

  const float scale = viewport_get_scale();
  const float *pan = viewport_get_pan();
  const float dpi = context_dpi();

  // update uvs
  texture_coo_uniform.scale[0] = base_tile[0];
  // base_tile[0] / texture_resolution / scale;
  texture_coo_uniform.scale[1] = base_tile[1];
  // base_tile[1] / texture_resolution / scale;

  const float denom = texture_resolution / grid_scale * 2.0f; // 25.6f

  texture_coo_uniform.offset[0] = fmodf(-1.0f * pan[0] / denom, 1.0f);
  //      -1.0f * pan[0] / scale / grid_scale / texture_resolution;
  texture_coo_uniform.offset[1] = fmodf(pan[1] / denom, 1.0f);
  //     pan[1] / scale / grid_scale / texture_resolution;

  rem_write_buffer(texture_coordinates_buffer, 0, &texture_coo_uniform,
                   sizeof(TextureCoordinatesUniform), REMWriteFlag_None);
}

void Widget::GridBackground::create_mesh() {

  // create mesh
  quad = rem_new_mesh();
  Primitive plane = primitive_plane();
  MeshCreatePrimitiveDescriptor quad_desc;
  quad_desc.primitive = &plane;
  quad_desc.name = "Background Quad";
  mesh_create_primitive(quad, &quad_desc);

  // NKENGINE TODO: Currently mesh_uniform() depends on UBO, so need to
  // manually input data find a way to update it without having to necessarily
  // link the mesh to the UBO.
  mesh_set_scale(quad, (vec3){2.0f, 2.0f, 2.0f}); // DPI ??
  MeshUniform mesh_uniform;
  glm_vec3_copy(quad->position, mesh_uniform.position);
  glm_mat4_copy(quad->model, mesh_uniform.model);

  WGPUBufferDescriptor buffer_desc = {};
  buffer_desc.label = "Grid Coordinates";
  buffer_desc.mappedAtCreation = false;
  buffer_desc.size = sizeof(TextureCoordinatesUniform);
  buffer_desc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
  texture_coordinates_buffer = rem_new_buffer(&buffer_desc);

  glm_vec4_copy((vec4){20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 1.0f},
                texture_coo_uniform.color);

  rem_write_buffer(texture_coordinates_buffer, 0, &texture_coo_uniform,
                   sizeof(TextureCoordinatesUniform), REMWriteFlag_None);

  update_uv();

  // assign shader
  ShaderCreateDescriptor shader_desc;
  shader_desc.name = "Background Quad Shader";
  shader_desc.pipeline = std_render_pipeline(RenderPipelineType_Screen);
  mesh_shader_create(quad, &shader_desc);

  // create dedicated sampler (repeat mode since ImGUi use Clamp and WGPU
  // backend doesn't offer any access to update it on init)
  WGPUSamplerDescriptor sampler_desc = {};
  sampler_desc.minFilter = WGPUFilterMode_Linear;
  sampler_desc.magFilter = WGPUFilterMode_Linear;
  sampler_desc.mipmapFilter = WGPUMipmapFilterMode_Linear;
  sampler_desc.addressModeU = WGPUAddressMode_Repeat;
  sampler_desc.addressModeV = WGPUAddressMode_Repeat;
  sampler_desc.addressModeW = WGPUAddressMode_Repeat;
  sampler_desc.maxAnisotropy = 1;

  // udpate bindgroup
  shader_update_uniform_data(mesh_shader(quad, MeshShader_Texture), 0, 0,
                             &mesh_uniform, ShaderUpdateFlag_None);

  shader_update_texture_view(mesh_shader(quad, MeshShader_Texture), 1, 0,
                             grid_view, WGPUTextureFormat_RGBA8Unorm,
                             ShaderUpdateFlag_None);

  shader_update_sampler(mesh_shader(quad, MeshShader_Texture), 1, 1,
                        &sampler_desc, ShaderUpdateFlag_None);

  shader_update_uniform_buffer(
      mesh_shader(quad, MeshShader_Texture), 1, 2, texture_coordinates_buffer,
      sizeof(TextureCoordinatesUniform), ShaderUpdateFlag_None);

  {
    draw_packet.mesh = quad;
    draw_packet.attribute = quad->topology.base.attribute.buffer;
    draw_packet.index = quad->topology.base.index.buffer;
    draw_packet.length = quad->topology.base.index.length;
    draw_packet.bindgroup_list =
        &mesh_shader(quad, MeshShader_Texture)->bind_groups;
    draw_packet.shader_name = mesh_shader(quad, MeshShader_Texture)->name;
  }
}

void Widget::GridBackground::create_grid_texture(
    const char *path, const TextureResolution resolution) {

  texture_resolution = resolution;

  base_tile[0] = (float)context_width() / texture_resolution * grid_scale;
  base_tile[1] = (float)context_height() / texture_resolution * grid_scale;

  WGPUTextureDescriptor texture_desc = {
      .label = "Background Grid Texture",
      .dimension = WGPUTextureDimension_2D,
      .format = TEXTURE_FORMAT_OFFSCREEN,
      .mipLevelCount = 1,
      .sampleCount = 1,
      .size = {resolution, resolution, 1},
      .usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding,
  };

  grid_texture = rem_new_texture(&texture_desc);

  Texture tex;
  TextureCreateFileDescriptor tex_desc = {
      .path = path,
      .channels = TextureChannel_RGBA,
      .flip = false,
      .width = resolution,
      .height = resolution,
  };
  texture_create_from_file(&tex, &tex_desc);

  rem_write_texture(grid_texture, tex.data, tex.size, tex.channels, 0,
                    REMWriteFlag_STBIFreeData);

  WGPUTextureViewDescriptor view_desc = {
      .label = "Background Grid Texture View",
      .arrayLayerCount = 1,
      .baseArrayLayer = 0,
      .mipLevelCount = 1,
      .aspect = WGPUTextureAspect_All,
      .dimension = WGPUTextureViewDimension_2D,
  };

  grid_view = rem_new_view(grid_texture, &view_desc);
}

// UNUSED: two costly, we just use a tiled texture
void Widget::GridBackground::draw() {
  ImDrawList *dl = ImGui::GetBackgroundDrawList();
  const ImGuiViewport *vp = ImGui::GetMainViewport();
  ImVec2 pos = vp->Pos;
  ImVec2 size = vp->Size;

  // Background fill
  dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_color);

  //  Grid dots (squares)
  if (viewport_get_scale() > 0.6) {

    const float scale = viewport_get_scale();
    const float inv_scale = 1.0f / scale;
    const float *pan = viewport_get_pan(); // Screen-space translation (pixels)
    const float half_dot_size_screen = dot_size * 0.5f;

    // World coordinates of the top-left screen corner
    // The transformation is: world = (screen - pan) / scale
    const float world_min_x = (pos.x - pan[0]) * inv_scale;
    const float world_min_y = (pos.y - pan[1]) * inv_scale;

    // World coordinates of the bottom-right screen corner
    const float world_max_x = (pos.x + size.x - pan[0]) * inv_scale;
    const float world_max_y = (pos.y + size.y - pan[1]) * inv_scale;

    // === 3. Snap the Iteration Start to the World Grid ===
    // We must start iterating at a position that is an exact multiple of
    // 'spacing', ensuring the grid lines always align perfectly to the world
    // origin (0,0).
    const float x_start_world = floorf(world_min_x / spacing) * spacing;
    const float y_start_world = floorf(world_min_y / spacing) * spacing;

    float x_w, y_w; // Variables for World Coordinates

    // === 4. Iterate and Draw ===
    for (x_w = x_start_world; x_w <= world_max_x; x_w += spacing) {
      // Convert world coordinate back to screen coordinate: screen = (world *
      // scale) + pan
      float x_screen = (x_w * scale) + pan[0];

      // Only draw if the center of the dot is actually visible on the screen
      // area.
      if (x_screen < pos.x || x_screen > pos.x + size.x)
        continue;

      for (y_w = y_start_world; y_w <= world_max_y; y_w += spacing) {
        float y_screen = (y_w * scale) + pan[1];

        if (y_screen < pos.y || y_screen > pos.y + size.y)
          continue;

        // p0 and p1 must be screen coordinates for AddRectFilled
        ImVec2 p0(x_screen - half_dot_size_screen,
                  y_screen - half_dot_size_screen);
        ImVec2 p1(x_screen + half_dot_size_screen,
                  y_screen + half_dot_size_screen);

        dl->AddRectFilled(p0, p1, dot_color, 0.0f);
      }
    }
  }
}

void Widget::GridBackground::draw_texture(WGPURenderPassEncoder pass_encoder) {

  const ImGuiViewport *vp = ImGui::GetMainViewport();
  ImVec2 pos = vp->Pos;
  ImVec2 size = vp->Size;
  ImDrawList *dl = ImGui::GetWindowDrawList();

  // dl->AddCallback();
  if (viewport_get_scale() > 0.6) {
    update_uv();
    WGPURenderPipeline pipeline =
        (*mesh_shader(quad, MeshShader_Texture)->pipeline)->handle;

    wgpuRenderPassEncoderSetPipeline(pass_encoder, pipeline);
    shader_draw(draw_packet.bindgroup_list, draw_packet.shader_name,
                pass_encoder);
    mesh_draw(&draw_packet, pass_encoder);
  } else {
    dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_color);
  }
}
