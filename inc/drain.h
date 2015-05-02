#ifndef _DRAIN_H_
#define _DRAIN_H_

#include "state_types.h"


#define BCM_DRAIN_ALLOWED(pack_v_min) (pack_v_min > MIN_CELL_V)

typedef enum {DRAIN_DRAINING, DRAIN_INIT, DRAIN_EXIT} DRAIN_MODE_T;
typedef enum {DRAIN_OK, DRAIN_ERROR, DRAIN_EXITED} DRAIN_STATUS_T;

DRAIN_STATUS_T Drain_Step(PACK_STATE_T*, MODE_T, OUTPUT_STATE_T*);
DRAIN_MODE_T Drain_GetMode(void);

#endif
