/**
 * @file drain.h
 * @author Eric Ponce
 * @date 25 June 2015
 * @brief Drain state machine
 */

#ifndef _DRAIN_H_
#define _DRAIN_H_

#include "state_types.h"
#include "a123mbb.h"

/**
 * @details Drain configuration struct
 * 
 * @param config Struct holding configuration information
 */
typedef struct {
	uint32_t min_cell_mVolts; ///<Minimum allowabe cell voltage
} DRAINING_CONFIG_T;

/**
 * @details Enum of drain states
 */
typedef enum {
	DRAIN_DRAINING, ///<Drain mode
	DRAIN_INIT, ///<Initialization mode
	DRAIN_OFF, ///<Not draiing
	DRAIN_DONE ///<Drain is done
} DRAIN_MODE_T;

/**
 * @details Configure the drain state machine
 * 
 * @param config configuration struct
 */
void Drain_Config(DRAINING_CONFIG_T *config);

/**
 * @details Step the drain state machine. Call as often as 
 * possible to keep drain state machine up to date
 * 
 * @param pack_state BCM state struct
 * @param req_mode requested mode
 * @param out_state Desired output state struct
 * @return Error state
 */
ERROR_T Drain_Step(PACK_STATE_T* pack_state, MODE_REQUEST_T req_mode, OUTPUT_STATE_T* out_state);

/**
 * @details Get the drain mode
 * @return Drain mode
 */
DRAIN_MODE_T Drain_GetMode(void);

#endif
