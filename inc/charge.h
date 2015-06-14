#ifndef _CHARGE_H_
#define _CHARGE_H_

#include "state_types.h"
#include "a123mbb.h"
#include "brusa.h"

#define BCM_BAL_ON_THRESH 1	    // As stated by datasheet, Value at which must stop charging
#define BCM_BAL_OFF_THRESH 4
#define BCM_BAL_CELL_V_MIN 3400
#define BCM_BAL_ENABLE(pack_v_max, pack_v_min) (((pack_v_max - pack_v_min) >= BCM_BAL_OFF_THRESH) && pack_v_min >= BCM_BAL_CELL_V_MIN)
#define BCM_BAL_DISABLE(pack_v_max, pack_v_min) (((pack_v_max - pack_v_min) <= BCM_BAL_ON_THRESH) || pack_v_min < BCM_BAL_CELL_V_MIN)


typedef enum {CHRG_OFF, CHRG_INIT, CHRG_CC, CHRG_CV, CHRG_DONE} CHARGING_MODE_T;

typedef struct {
	uint8_t pack_s; 				// Number of cells in series
	uint8_t pack_p;					// Number of cells in parallel
	uint32_t max_cell_mVolts; 		// Maximum cell voltage
	uint32_t cc_cell_mVolts;		// Voltage over cell in CC Mode
	uint16_t cell_capacity_cAmpHours;
	uint8_t cell_mC_rating;
} CHARGING_CONFIG_T;

void Charge_Config(CHARGING_CONFIG_T *config);
ERROR_T Charge_Step(PACK_STATE_T*, MODE_REQUEST_T, OUTPUT_STATE_T*);
CHARGING_MODE_T Charge_GetMode(void);

#endif
