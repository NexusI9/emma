#ifndef _EMMA_HEAT_MAP_H_
#define _EMMA_HEAT_MAP_H_

#include "nkengine/include/utils.h"
#include "runtime/manager/allocator.h"
#include "runtime/manager/allocator_list.h"

typedef struct Heatmap Heatmap;

typedef void (*heatmap_color_mapper_callback)(Heatmap *, Frame *, void *,
                                              color);

typedef enum {
  HeatmapStatus_Success,
  HeatmapStatus_UndefError,
} HeatmapStatus;

struct Heatmap {

  color background;
  AllocIdRefList frames;

  WGPUTexture texture;
  WGPUTextureView views[2];

  heatmap_color_mapper_callback color_mapper;
};

typedef struct {
  color background;
  AllocIdRefList frames;
  int width, height;
  heatmap_color_mapper_callback color_mapper;
} HeatmapDescriptor;

EXTERN_C_BEGIN

HeatmapStatus heatmap_create(Heatmap *, const HeatmapDescriptor *);

EXTERN_C_END

#endif
