#ifndef _TYPES_H_
#define _TYPES_H_

#include "lpc_types.h"


// ------------------------------------------------
// Structs and Enum

typedef enum {IDLE, CHARGING, DRAINING} MODE_T;
typedef enum {REQ_IDLE, REQ_CHARGING, REQ_DRAINING, REQ_NONE} MODE_REQUEST_T;
typedef enum {CHRG_BALANCING, CHRG_CHARGING, CHRG_NONE} CHARGING_MODE_T;

typedef struct {
	uint32_t pack_v_min;
	uint8_t  pack_node_min;
	uint32_t pack_v_max;
	uint8_t  pack_node_max;
	uint32_t pack_v_avg;
	uint32_t messagesReceived;
} PACK_STATE;


#endif