#ifndef _DRAIN_H_
#define _DRAIN_H_

#include "types.h"
#define BCM_DRAIN_ALLOWED(pack_v_min) (pack_v_min > MIN_CELL_V)

typedef enum {DRAIN_DRAINING, DRAIN_INIT, DRAIN_EXIT} DRAIN_MODE_T;
typedef enum {DRAIN_OK, DRAIN_ERROR, DRAIN_EXITED} DRAIN_STATUS_T;

DRAIN_STATUS_T Drain_Step(PACK_STATE*, MODE_T, OUTPUT_STATE*);
DRAIN_MODE_T Charge_GetMode(void);

#endif
