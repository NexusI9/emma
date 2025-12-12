#ifndef _UTILS_CALLBACK_H_
#define _UTILS_CALLBACK_H_

#define CALLBACK_ENTRY(Type)                                                   \
  struct {                                                                     \
    Type callback;                                                             \
    void *data;                                                                \
  }

#endif
