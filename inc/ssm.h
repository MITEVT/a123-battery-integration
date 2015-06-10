#ifndef _SSM_H_
#define _SSM_H_

#include "state_types.h"
#include "charge.h"
#include "drain.h"

void SSM_Init(void);
ERROR_T SSM_Step(PACK_STATE_T *pack_state, MODE_INPUT_T inp, OUTPUT_STATE_T *out_state);
MODE_T SSM_GetMode(void);

#endif