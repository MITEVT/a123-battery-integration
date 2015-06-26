/**
 * @file charge.h
 * @author Eric Ponce
 * @date 25 June 2015
 * @brief Charge state machine
 */

#ifndef _CHARGE_H_
#define _CHARGE_H_

#include "state_types.h"
#include "a123mbb.h"
#include "brusa.h"

#define BCM_BAL_ON_THRESH 1	    ///<Threshold at which balancing turns off
#define BCM_BAL_OFF_THRESH 4 	///<Threshold at which balancing turns on
#define BCM_BAL_CELL_V_MIN 3400 ///<Minimum balance voltage

/// Returns true if balancing should be enabled
#define BCM_BAL_ENABLE(pack_v_max, pack_v_min) (((pack_v_max - pack_v_min) >= BCM_BAL_OFF_THRESH) && pack_v_min >= BCM_BAL_CELL_V_MIN)
/// Returns true if balancing should be disabled
#define BCM_BAL_DISABLE(pack_v_max, pack_v_min) (((pack_v_max - pack_v_min) <= BCM_BAL_ON_THRESH) || pack_v_min < BCM_BAL_CELL_V_MIN)

/**
 * Enum describing charging state
 */
typedef enum {
	CHRG_OFF, ///<Not charging
	CHRG_INIT, ///<State machine is initializing
	CHRG_CC, ///<Charging in constant current mode
	CHRG_CV, ///<Charging in constant voltage mode
	CHRG_DONE ///<Charging is done
} CHARGING_MODE_T;

/**
 * Configuration struct
 */
typedef struct {
	uint8_t pack_s; 				///<Number of cells in series
	uint8_t pack_p;					///<Number of cells in parallel
	uint32_t max_cell_mVolts; 		///<Maximum cell voltage in millivolts
	uint32_t cc_cell_mVolts;		///<Voltage over cell in CC Mode in millivolts
	uint16_t cell_capacity_cAmpHours; ///<Cell capacity in centiamp hours
	uint16_t cell_mC_rating; 		///<Cell C Rating in thousands of C
} CHARGING_CONFIG_T;

/**
 * @details Configure and reset charge state machine
 * 
 * @param config Configuration struct
 */
void Charge_Config(CHARGING_CONFIG_T *config);

/**
 * @details Steps the charge state machine. Should be called as often as possible 
 * so as to keep its information up to date. Nominally in mode CHRG_OFF
 * 
 * @param pack_state Current state struct
 * @param req_mode requested mode
 * @param out_state state machine output request struct
 * @return error information
 */
ERROR_T Charge_Step(PACK_STATE_T* pack_state, MODE_REQUEST_T req_mode, OUTPUT_STATE_T* out_state);

/**
 * @details Gets the current charge state machine mode.
 */

CHARGING_MODE_T Charge_GetMode(void);

#endif
