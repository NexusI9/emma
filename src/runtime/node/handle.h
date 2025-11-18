#ifndef _HANDLE_H_
#define _HANDLE_H_

#include <cglm/cglm.h>

#include "utils/id.h"

typedef enum {
  HandleStatus_Success,
  HandleStatus_UndefError,
} HandleStatus;


typedef struct {

  alloc_id id;
  vec2 start;
  vec2 end;

} Handle;

#endif
