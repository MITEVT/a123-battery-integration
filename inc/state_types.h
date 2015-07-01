/**
 * @file state_types.h
 * @author Eric Ponce
 * @date 25 June 2015
 * @brief Defines globally used types
 */

#ifndef _TYPES_H_
#define _TYPES_H_

// #include "lpc_types.h"
#include <stdint.h>
#include <stdbool.h>
#include "brusa.h"

// ------------------------------------------------
// Structs and Enum

/**
 * Enum of Error types
 */
typedef enum {	
	ERROR_NONE = 0, ///<No Error
	ERROR_LOW_VOLTAGE = 1, ///<Low Pack Voltage Error
	ERROR_HIGH_VOLTAGE = 2, ///<High Pack Voltage Error
	ERROR_CAN_BUS = 3, ///<Can bus error
	ERROR_INCOMPATIBLE_MODE = 4, ///<Incompatible mode error
	ERROR_CONTACTOR = 5, ///<Contactor circuitry error
	ERROR_CHARGE_SM = 6, ///<Charge state machine error
	ERROR_BRUSA = 7
	} ERROR_T; 

/**
 * Enum of BCM Modes
 */
typedef enum {
	IDLE, ///<Idle mode
	CHARGING, ///<Charging mode
	DRAINING ///<Drain mode
} MODE_T;

/**
 * Enum of BCM Mode requests
 */
typedef enum {
	REQ_IDLE, ///<Request to Idle
	REQ_CHARGING, ///<Request to Charge
	REQ_DRAINING, ///<Request to Drain
	REQ_NONE ///<No request
} MODE_REQUEST_T;

/**
 * Enum of Mode Inputs
 */
typedef enum {
	INP_CHRG, ///<Input for Charging
	INP_DRAIN, ///<Input for Draining
	INP_IDLE ///<Input for Idle
} MODE_INPUT_T;

/**
 * Struct containing the current state of the BCM
 */
typedef struct {
	uint32_t pack_min_mVolts; ///<Lowest cell voltage in pack
	uint32_t pack_max_mVolts; ///<Highest cell voltage in pack
	uint16_t pack_cAmps_in; ///<Current being supplied by Brusa into Pack
	bool contactors_closed; ///<State of contactors
	volatile uint64_t msTicks; ///<Current millisecond count of processor
	NLG5_ERR_T brusa_error; ///<Brusa error state
} PACK_STATE_T;

/**
 * Struct containing desired output state information
 */
typedef struct {
	bool balance; ///<Request to balance
	uint16_t balance_mVolts; ///<Balance voltage
	bool brusa_output; ///<Request for brusa communication
	bool brusa_clear_latch; ///<Clear brusa error latch
	uint32_t brusa_mVolts; ///<Desired brusa output voltage limit
	uint32_t brusa_cAmps; ///<Desired brusa output current limit
	bool close_contactors; ///<Desired state of contactors
} OUTPUT_STATE_T;


#endif