#ifndef _CHARGE_H_
#define _CHARGE_H_

#include "state_types.h"
#include "a123mbb.h"
#include "brusa.h"

#define BCM_BAL_ON_THRESH 1/2	    // As state by datasheet, Value at which must stop charging
#define BCM_BAL_OFF_THRESH 4/2
#define BCM_BAL_CELL_V_MIN mVolts2Num(3400)
#define BCM_BAL_ENABLE(pack_v_max, pack_v_min) ((pack_v_max - pack_v_min) > BCM_BAL_OFF_THRESH) && pack_v_min >= BCM_BAL_CELL_V_MIN
#define BCM_BAL_DISABLE(pack_v_max, pack_v_min) ((pack_v_max - pack_v_min) <= BCM_BAL_ON_THRESH) || pack_v_min < BCM_BAL_CELL_V_MIN

#define BCM_CHRG_CC_CELL_V_MAX mVolts2Num(3600)
#define BCM_CHRG_STATE(pack_v_max) ((pack_v_max < BCM_CHRG_CC_CELL_V_MAX) ? CHRG_CC : CHRG_CV)

#define BCM_CHRG_CC_BRUSA_DECI_V 37
#define BCM_CHRG_CV_BRUSA_DECI_V 36


typedef enum {CHRG_OFF, CHRG_INIT, CHRG_CC, CHRG_CV, CHRG_DONE} CHARGING_MODE_T;
typedef enum {CHRG_OK, CHRG_ERROR} CHARGING_STATUS_T;

typedef struct {
	uint8_t mod_s;
	uint8_t mod_p;
	uint32_t max_deci_amps_per_p;
} CHARGING_CONFIG_T;

void Charge_Config(CHARGING_CONFIG_T *config);
CHARGING_STATUS_T Charge_Step(PACK_STATE_T*, MODE_REQUEST_T, OUTPUT_STATE_T*);
CHARGING_MODE_T Charge_GetMode(void);

#endif
