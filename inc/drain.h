#ifndef _DRAIN_H_
#define _DRAIN_H_

#include "state_types.h"
#include "a123mbb.h"

typedef struct {
	uint32_t min_cell_mVolts;
} DRAINING_CONFIG_T;

typedef enum {DRAIN_DRAINING, DRAIN_INIT, DRAIN_OFF, DRAIN_DONE} DRAIN_MODE_T;

void Drain_Config(DRAINING_CONFIG_T *config);
ERROR_T Drain_Step(PACK_STATE_T*, MODE_REQUEST_T, OUTPUT_STATE_T*);
DRAIN_MODE_T Drain_GetMode(void);

#endif
