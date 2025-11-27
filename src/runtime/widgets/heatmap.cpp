#include "heatmap.hpp"
#include "nkengine/include/compute.h"
#include "nkengine/include/context.h"
#include "nkengine/include/gui.hpp"
#include "runtime/manager/allocator.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/viewport.h"
#include "runtime/node/heatmap.h"
#include "runtime/widgets/module.hpp"
#include "webgpu/webgpu.h"
#include <cstdlib>
#include <imgui/imgui.h>

void Widget::HeatmapShape::compute_offline(WGPUCommandEncoder encoder) {

  const float *pan = viewport_get_pan();
  init_offset.x = pan[0];
  init_offset.y = pan[1];

  // draw zones with intensity
  compute_render_pass(encoder);

  // convert onscreen (BGRA) => offscreen (RGBA) for compute
  compute_pass_bgra2rgba_draw_inline(
      &node->compute_passes[HeatmapComputePass_BGRA2RGBA], encoder);

  // apply blur
  compute_pass_kawase_draw_inline(
      &node->compute_passes[HeatmapComputePass_Kawase], node->blur, encoder);

  // remap blur to heatmap configuration colors
  compute_pass_colormap_draw_inline(
      &node->compute_passes[HeatmapComputePass_Colormap], encoder);
}

void Widget::HeatmapShape::compute_render_pass(WGPUCommandEncoder encoder) {

  WGPURenderPassColorAttachment color = {};
  color.clearValue = {0};
  color.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
  color.loadOp = WGPULoadOp_Clear;
  color.storeOp = WGPUStoreOp_Store;
  color.view = node->views[HeatmapTexture_Color_Onscreen];

  WGPURenderPassDepthStencilAttachment depth = {};
  depth.depthClearValue = 0;
  depth.depthReadOnly = true;
  depth.view = node->views[HeatmapTexture_Depth];

  WGPURenderPassDescriptor pass_desc = {};
  pass_desc.colorAttachmentCount = 1;
  pass_desc.colorAttachments = &color;
  pass_desc.depthStencilAttachment = &depth;
  pass_desc.label = "Heatmap Render Pass";

  WGPURenderPassEncoder pass =
      wgpuCommandEncoderBeginRenderPass(encoder, &pass_desc);

  ImGui_ImplWGPU_NewFrame();
  ImGui::NewFrame();

  UI::FullScreenWindow().Begin("Heatmap Offscreen");

  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImGuiViewport *vp = ImGui::GetMainViewport();

  // dl->AddRectFilled(ImVec2(0, 0), vp->Size, ImColor(0, 0, 0, 255));

  // draw each modules
  for (size_t i = 0; i < *node->frames.length; i++) {
    Frame *parent_frame = allocator_frame_entry(node->frames.entries[i]);

    for (size_t j = 0; j < parent_frame->children.length; j++) {
      Frame *module = allocator_frame_entry(parent_frame->children.entries[j]);

      // float module_intensity = node->intensity_mapper(node, module, NULL);
      int rd_r = ((float)rand() / RAND_MAX) * 255;
      int rd_g = ((float)rand() / RAND_MAX) * 255;
      int rd_b = ((float)rand() / RAND_MAX) * 255;

      ModuleShape(module).draw_fill(ImColor(rd_r, rd_g, rd_b, 255),
                                    node->scale);
    }
  }

  UI::FullScreenWindow().End();

  ImGui::Render();
  ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);

  wgpuRenderPassEncoderEnd(pass);
}

void Widget::HeatmapShape::draw() {

  ImDrawList *dl = ImGui::GetWindowDrawList();

  // TODO: cache vp size ?
  ImGuiViewport *vp = ImGui::GetMainViewport();

  // bg
  dl->AddRectFilled(ImVec2(0, 0), vp->Size, im_color(node->background));

  // baked heatmap
  const float *pan = viewport_get_pan();
  dl->AddImage((ImTextureRef)node->views[HeatmapTexture_Color_Offscreen],
               ImVec2(pan[0] - init_offset.x, pan[1] - init_offset.y),
               ImVec2(pan[0] + vp->Size.x - init_offset.x,
                      pan[1] + vp->Size.y - init_offset.y),
               ImVec2(0, 0), ImVec2(1, 1));

  draw_gradient(dl);
}

void Widget::HeatmapShape::draw_gradient(ImDrawList *dl) {

  ImGui::BeginGroup();

  gradient.draw_vertical(dl);

  static constexpr ImVec2 line_top_end =
      ImVec2(gradient_anchor.x + 40, gradient_anchor.y);

  static constexpr ImVec2 line_bottom_start =
      ImVec2(gradient_anchor.x, gradient_anchor.y + gradient_height);

  static constexpr ImVec2 line_bottom_end =
      ImVec2(gradient_anchor.x + 40, gradient_anchor.y + gradient_height);

  ImGui::SetCursorPos(gui_scale_im_vec2(
      gui, ImVec2(gradient_anchor.x, gradient_anchor.y - 50)));

  ImGui::SetWindowFontScale(1.5f);
  ImGui::Text("%s", node->label);
  ImGui::SetWindowFontScale(1.0f);

  draw_gradient_legend(dl, gradient_anchor, line_top_end, node->axes[0]);
  draw_gradient_legend(dl, line_bottom_start, line_bottom_end, node->axes[1]);

  ImGui::EndGroup();
}

void Widget::HeatmapShape::draw_gradient_legend(ImDrawList *dl, ImVec2 start,
                                                ImVec2 end, const char *label) {

  static const int line_value = 140;
  dl->AddLine(gui_scale_im_vec2(gui, start), gui_scale_im_vec2(gui, end),
              ImColor(line_value, line_value, line_value, 255), 3.0f);

  ImGui::SetCursorPos(gui_scale_im_vec2(gui, ImVec2(end.x + 10, end.y - 8)));

  ImGui::Text("%s", label);
}
