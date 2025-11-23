#ifndef _TRANSFORM_HANDLE_H_
#define _TRANSFORM_HANDLE_H_

#include <cglm/cglm.h>

#include "nkengine/include/utils.h"
#include "utils/id.h"

typedef enum {
  TransformHandleStatus_Success,
  TransformHandleStatus_UndefError,
} TransformHandleStatus;

/*

   TL                    TM                  TR
       o-----------------o-----------------o
       |                 |                 |
       |                 |                 |
       |                 MM                |
   ML  o-----------------o-----------------o  MR
       |                 |                 |
       |                 |                 |
       |                 |                 |
       o-----------------o-----------------o
   BL                    BM                  BR

 */
typedef enum {
  TransformHandleType_TL, // Top Left
  TransformHandleType_TM, // Top Middle
  TransformHandleType_TR, // Top Right
  TransformHandleType_ML, // Middle Left
  TransformHandleType_MM, // Middle Middle
  TransformHandleType_MR, // Middle Right
  TransformHandleType_BL, // Bottom Left
  TransformHandleType_BM, // Bottom Middle
  TransformHandleType_BR, // Bottom Left
} TransformHandleType;

typedef struct {
  alloc_id id;
  vec2 position;
  float scale;
  color color, stroke_color;
  float stroke_width;
} TransformHandle;

typedef struct {
  const vec2 position;
  const float scale;
  const color color, stroke_color;
  const float stroke_width;
} TransformHandleDescriptor;

EXTERN_C_BEGIN

void transform_handle_create(TransformHandle *,
                             const TransformHandleDescriptor *);

EXTERN_C_END

#endif
