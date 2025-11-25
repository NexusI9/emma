#ifndef _EMMA_HEAT_MAP_H_
#define _EMMA_HEAT_MAP_H_

#include "nkengine/include/utils.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"

typedef struct Heatmap Heatmap;

typedef float (*heatmap_intensity_map_callback)(Heatmap *, Frame *, void *);

typedef enum {
  HeatmapStatus_Success,
  HeatmapStatus_UndefError,
} HeatmapStatus;

struct Heatmap {

  color background;
  AllocIdRefList frames;

  WGPUTexture texture;
  WGPUTextureView views[2];

  heatmap_intensity_map_callback intensity_mapper;
};

typedef struct {
  color background;
  AllocIdRefList frames;
  int width, height;
  heatmap_intensity_map_callback intensity_mapper;
} HeatmapDescriptor;

EXTERN_C_BEGIN

HeatmapStatus heatmap_create(Heatmap *, const HeatmapDescriptor *);

EXTERN_C_END

#endif
