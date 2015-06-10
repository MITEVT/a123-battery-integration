#include "ssm.h"

static MODE_T mode;

void SSM_Init(void) {
	mode = IDLE;
}

ERROR_T SSM_Step(PACK_STATE_T *pack_state, MODE_INPUT_T inp, OUTPUT_STATE_T *out_state) {
	if (inp == INP_IDLE) {
		if (mode != IDLE) {
			// Move to IDLE
		} else {
			return ERROR_NONE;
		}
	} else if (inp == INP_CHRG) {
		if (mode != CHARGING) {
			// Move to CHARGING
			if (mode == IDLE) {
				Charge_Step(pack_state, REQ_CHARGING, out_state);
			} else {
				// Stop Draining then Charge
				// while (Draining) {
				// 	Stop Draining
				// }
				Charge_Step(pack_state, REQ_CHARGING, out_state);
			}
			mode = CHARGING;
		} else {
			// Keep Charging
			CHARGING_STATUS_T status = Charge_Step(pack_state, REQ_NONE, out_state);
			if (status == CHRG_ERROR) {
				return ERROR_CHARGE_SM;
			} else {
				return ERROR_NONE;
			}
		}
	} else if (inp == INP_DRAIN) {
		if (mode != DRAINING) {
			// Move to Draining
		} else {
			// Keep Draining
		}
	} else {
		return ERROR_INCOMPATIBLE_MODE;
	}

	return ERROR_NONE;
}

MODE_T SSM_GetMode(void) {
	return mode;
}


