#ifndef _DRAIN_H_
#define _DRAIN_H_

#include "types.h"

#define BCM_DRAIN_ALLOWED(pack_v_min) (pack_v_min > MIN_CELL_V)

typedef enum {DRAIN_OK, DRAIN_ERROR, DRAIN_EXITED} DRAIN_STATUS_T;

DRAINING_STATUS_T Drain_Step(PACK_STATE pack_state, MODE_T next_mode, OUTPUT_STATE*);
DRAINING_MODE_T Drain_GetMode(void);

#endif
