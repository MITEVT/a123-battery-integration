#ifndef _DRAIN_H_
#define _DRAIN_H_

#include "types.h"

#define BCM_BAL_ON_THRESH mVolts2Num(1)	    // As state by datasheet, Value at which must stop charging
#define BCM_BAL_OFF_THRESH mVolts2Num(4)
#define BCM_BAL_CELL_V_MIN mVolts2Num(3400)
#define BCM_CHRG_CELL_V_MAX mVolts2Num(3600)

#define BCM_CHRG_ALLOWED(pack_v_max) (pack_v_max < BCM_CHRG_CELL_V_MAX)

#define BCM_BAL_ENABLE(pack_v_max, pack_v_min) ((pack_v_max - pack_v_min) > BCM_BAL_OFF_THRESH) && pack_v_min > BCM_BAL_CELL_V_MIN
#define BCM_BAL_DISABLE(pack_v_max, pack_v_min) ((pack_v_max - pack_v_min) <= BCM_BAL_ON_THRESH) || pack_v_min < BCM_BAL_CELL_V_MIN

typedef enum {CHRG_CHARGING, CHRG_INIT, CHRG_EXIT} CHARGING_MODE_T;
typedef enum {CHRG_OK, CHRG_ERROR, CHRG_EXITED} CHARGING_STATUS_T;

DRAIN_STATUS_T Drain_Step(PACK_STATE*, MODE_T, OUTPUT_STATE*);
DRAIN_MODE_T Charge_GetMode(void);

#endif