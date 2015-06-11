#ifndef _DRAIN_H_
#define _DRAIN_H_

#include "state_types.h"
#include "a123mbb.h"


#define BCM_DRAIN_ALLOWED(pack_v_min) (pack_v_min > MIN_CELL_V)

typedef enum {DRAIN_DRAINING, DRAIN_INIT, DRAIN_OFF, DRAIN_DONE} DRAIN_MODE_T;

ERROR_T Drain_Step(PACK_STATE_T*, MODE_REQUEST_T, OUTPUT_STATE_T*);
DRAIN_MODE_T Drain_GetMode(void);

#endif
