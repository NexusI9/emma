#ifndef _HANDLE_H_
#define _HANDLE_H_

#include <cglm/cglm.h>

#include "utils/id.h"

typedef enum {
  HandleStatus_Success,
  HandleStatus_UndefError,
} HandleStatus;

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
  HandleType_TL, // Top Left
  HandleType_TM, // Top Middle
  HandleType_TR, // Top Right

  HandleType_ML, // Middle Left
  HandleType_MM, // Middle Middle
  HandleType_MR, // Middle Right

  HandleType_BL, // Bottom Left
  HandleType_BM, // Bottom Middle
  HandleType_BR, // Bottom Left

} HandleType;

typedef struct {
  alloc_id id;
  vec2 position;
  vec2 scale;
} Handle;

#endif
