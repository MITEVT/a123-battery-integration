#ifndef _TYPES_H_
#define _TYPES_H_

// #include "lpc_types.h"
#include <stdint.h>
#include <stdbool.h>

// ------------------------------------------------
// Structs and Enum
typedef enum {	ERROR_NONE = 0,
				ERROR_LOW_VOLTAGE = 1,
				ERROR_HIGH_VOLTAGE = 2,
				ERROR_CAN_BUS = 3,
				ERROR_INCOMPATIBLE_MODE = 4,
				ERROR_CONTACTOR = 5,
				ERROR_CHARGE_SM = 6} ERROR_T;

typedef enum {IDLE, CHARGING, DRAINING} MODE_T;
typedef enum {REQ_IDLE, REQ_CHARGING, REQ_DRAINING, REQ_NONE} MODE_REQUEST_T;
typedef enum {INP_CHRG, INP_DRAIN, INP_IDLE} MODE_INPUT_T;

typedef struct {
	uint32_t pack_min_mVolts;
	uint8_t  pack_node_min;
	uint32_t pack_max_mVolts;
	uint8_t  pack_node_max;
	uint32_t pack_avg_mVolts;
	uint32_t messagesReceived;
	uint16_t pack_cAmps_in;
	bool contactors_closed;
	volatile uint64_t msTicks;
} PACK_STATE_T;

typedef struct {
	uint16_t balance_mVolts;
	uint32_t brusa_mVolts;
	uint16_t brusa_cAmps;
	bool close_contactors;
} OUTPUT_STATE_T;


#endif