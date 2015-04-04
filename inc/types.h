#ifndef _TYPES_H_
#define _TYPES_H_

// #include "lpc_types.h"
#include "chip.h"
#include "mcp2515.h"
#include "brusa.h"
#include "a123mbb.h"


#define MAX_CELL_V 	mVolts2Num(3800) 		// Maximum Allowed Cell Voltage of 3.8V
#define MIN_CELL_V  mVolts2Num(2700) 		// Minimum Allowed Cell Voltage of 2.7V

// ------------------------------------------------
// Structs and Enum

typedef enum {IDLE, CHARGING, DRAINING} MODE_T;
typedef enum {REQ_IDLE, REQ_CHARGING, REQ_DRAINING, REQ_NONE} MODE_REQUEST_T;

typedef struct {
	uint32_t pack_v_min;
	uint8_t  pack_node_min;
	uint32_t pack_v_max;
	uint8_t  pack_node_max;
	uint32_t pack_v_avg;
	uint32_t messagesReceived;
	bool contactors_closed;
} PACK_STATE;

typedef struct {
	uint16_t balance_voltage;
	uint16_t brusa_voltage;
	uint16_t brusa_current;
	bool close_contactors;
} OUTPUT_STATE;


#endif