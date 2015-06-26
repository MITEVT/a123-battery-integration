/**
 * @file ssm.h
 * @author Eric Ponce
 * @date 25 June 2015
 * @brief Super state machine
 */

#ifndef _SSM_H_
#define _SSM_H_

#include "state_types.h"
#include "charge.h"
#include "drain.h"

/**
 * @details Initialize and reset the super state machine
 */
void SSM_Init(void);

/**
 * @details Step the super state machine. Call as 
 * often as possible to keep all sub states up to date.
 * 
 * @param pack_state Current BCM state struct
 * @param inp system input
 * @param out_state Desired output state struct
 * @return Error state
 */
ERROR_T SSM_Step(PACK_STATE_T *pack_state, MODE_INPUT_T inp, OUTPUT_STATE_T *out_state);

/**
 * @details Get super state machine mode
 * @return Super state mode
 */
MODE_T SSM_GetMode(void);

#endif