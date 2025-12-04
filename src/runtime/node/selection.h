#ifndef _EMMA_SELECTION_H_
#define _EMMA_SELECTION_H_

#include <stdbool.h>

typedef enum {
  // No click emitted during the session
  SelectionStatus_Off,

  // Click was emitted but no hit
  SelectionStatus_Blank,

  // Click was emitted and hit
  SelectionStatus_Hit,
} SelectionStatus;

typedef struct {

  SelectionStatus status;
  int click_count;
  int hit_count;

} Selection;

static inline void selection_init(Selection *selection) {
  selection->click_count = 0;
  selection->hit_count = 0;
  selection->status = SelectionStatus_Off;
}

static inline bool selection_blank(Selection *selection, bool trigger) {

  // we don't reset back to blank if already hit during the session.
  if (trigger && selection->status != SelectionStatus_Hit) {
    selection->click_count++;
    selection->status = SelectionStatus_Blank;
  }

  return trigger;
}

static inline bool selection_hit(Selection *selection, bool trigger) {

  if (trigger) {
    selection->click_count++;
    selection->hit_count++;
    selection->status = SelectionStatus_Hit;
  }

  return trigger;
}

static inline bool selection_begin(Selection *selection, bool trigger) {

  return selection_blank(selection, trigger);
}

static inline SelectionStatus selection_end(Selection *selection) {

  if (selection->status == SelectionStatus_Blank) {
    selection->click_count = 0;
    selection->hit_count = 0;
  }

  selection->status = SelectionStatus_Off;

  return selection->status;
}

static inline SelectionStatus selection_status(Selection *selection) {
  return selection->status;
}

#endif
