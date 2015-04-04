#ifndef _DRAIN_H_
#define _DRAIN_H_

#include "types.h"

typedef enum {VALID, ERROR} DRAIN_RET_STATUS_T;
uint8_t stepDrain(PACK_STATE pack_state, MODE_T next_mode);

#endif
