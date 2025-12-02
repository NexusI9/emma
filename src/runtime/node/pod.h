#ifndef _EMMA_POD_H_
#define _EMMA_POD_H_

#include "runtime/node/frame.h"

typedef enum {
  PodStatus_Success,
  PodStatus_UndefError,
} PodStatus;

Frame *pod_create();

#endif
