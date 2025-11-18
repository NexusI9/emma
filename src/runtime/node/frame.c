#include "frame.h"

FrameStatus frame_create(Frame *node, const FrameDescriptor *desc) {

  frame_set_size(node, desc->size);
  frame_set_position(node, desc->position);
  frame_set_background(node, desc->background);

  return FrameStatus_Success;
}
