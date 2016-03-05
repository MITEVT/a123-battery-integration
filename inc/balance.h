/**
 * @file balance.h
 * @author Eric Ponce
 * @date 5 March 2016
 * @brief Balance state machine
 */

#ifndef _BALANCE_H_
#define _BALANCE_H_

#include "state_types.h"
#include "a123mbb.h"
#include "brusa.h"

#define BCM_BAL_ON_THRESH 1	    ///<Threshold at which balancing turns off
#define BCM_BAL_OFF_THRESH 4 	///<Threshold at which balancing turns on

/// Returns true if balancing should be enabled
#define BAL_ENABLE(pack_v_max, pack_v_min) (((pack_v_max - pack_v_min) >= BCM_BAL_OFF_THRESH))
/// Returns true if balancing should be disabled
#define BAL_DISABLE(pack_v_max, pack_v_min) (((pack_v_max - pack_v_min) <= BCM_BAL_ON_THRESH))

/**
 * Enum describing balancing state
 */
typedef enum {
	BAL_OFF, ///<Not charging
	BAL_INIT, ///<State machine is initializing
	BAL_ON, ///<Balancing
	BAL_DONE ///<Balancing is done
} BALANCING_MODE_T;

/**
 * @details Steps the balance state machine. Should be called as often as possible 
 * so as to keep its information up to date. Nominally in mode BAL_OFF
 * 
 * @param pack_state Current state struct
 * @param req_mode requested mode
 * @param out_state state machine output request struct
 * @return error information
 */
ERROR_T Balance_Step(PACK_STATE_T* pack_state, MODE_REQUEST_T req_mode, OUTPUT_STATE_T* out_state);

/**
 * @details Gets the current balance state machine mode.
 */
BALANCING_MODE_T Balance_GetMode(void);

#endif
