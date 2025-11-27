#ifndef _EMMA_HEAT_MAP_H_
#define _EMMA_HEAT_MAP_H_

#include "nkengine/include/compute.h"
#include "nkengine/include/renderer.h"
#include "nkengine/include/utils.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"

typedef struct Heatmap Heatmap;

typedef enum {
  // Used to draw imGui (BGRA)
  HeatmapTexture_Color_Onscreen,
  // Used to compute passes (RGBA)
  HeatmapTexture_Color_Offscreen,
  // Needed by imGui
  HeatmapTexture_Depth,
  HeatmapTexture_COUNT,
} HeatmapTexture;

typedef enum {
  HeatmapComputePass_BGRA2RGBA,
  HeatmapComputePass_Kawase,
  HeatmapComputePass_Colormap,
  HeatmapComputePass_COUNT,
} HeatmapComputePass;

typedef float (*heatmap_intensity_map_callback)(Heatmap *, Frame *, void *);

typedef enum {
  HeatmapStatus_Success,
  HeatmapStatus_UndefError,
} HeatmapStatus;

struct Heatmap {

  const char *axes[2];
  const char *label;

  color background;
  AllocIdRefList frames;
  uint8_t blur;
  float scale;

  ColormapUniform color_map;

  WGPUTexture textures[HeatmapTexture_COUNT];
  WGPUTextureView views[HeatmapTexture_COUNT];
  ComputePass compute_passes[HeatmapComputePass_COUNT];

  heatmap_intensity_map_callback intensity_mapper;
};

typedef struct {
  color background;
  AllocIdRefList frames;
  int width, height;
  heatmap_intensity_map_callback intensity_mapper;
  uint8_t blur;
  float scale;
  ColormapUniform *color_map;
  const char *axes[2];
  const char *label;
} HeatmapDescriptor;

EXTERN_C_BEGIN

HeatmapStatus heatmap_create(Heatmap *, const HeatmapDescriptor *);

EXTERN_C_END

#endif
