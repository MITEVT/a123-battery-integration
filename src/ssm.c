#include "ssm.h"

static MODE_T mode;

void SSM_Init(void) {
	mode = IDLE;
}

ERROR_T SSM_Step(PACK_STATE_T *pack_state, MODE_INPUT_T inp, OUTPUT_STATE_T *out_state) {
	if (inp == INP_IDLE) {
		if (mode != IDLE) {
			// Move to IDLE
			if (mode == CHARGING) {
				if (Charge_GetMode() == CHRG_OFF) {
					mode = IDLE;
					return ERROR_NONE;
				}

				return Charge_Step(pack_state, REQ_IDLE, out_state);
			} else {
				// [TODO] Implement Drain to Idle Logic
				if (Drain_GetMode() == DRAIN_OFF) {
					mode = IDLE;
					return ERROR_NONE;
				}
				return Drain_Step(pack_state, REQ_IDLE, out_state);
				
			}
		} else {
			return ERROR_NONE;
		}
	} else if (inp == INP_CHRG) {
		if (mode != CHARGING) {
			// Move to CHARGING
			if (mode == IDLE) {
				mode = CHARGING;
				return Charge_Step(pack_state, REQ_CHARGING, out_state);
			} else {
				// [TODO] Drain 2 Idle
				if (Drain_GetMode() == DRAIN_OFF) {
					mode = IDLE;
					return ERROR_NONE;
				}
				return Drain_Step(pack_state, REQ_IDLE, out_state);
			}
		} else {
			// Keep Charging
			return Charge_Step(pack_state, REQ_NONE, out_state);
		}
	} else if (inp == INP_DRAIN) {
		if (mode != DRAINING) {
			// Move to Draining
			if (mode == IDLE) {
				mode = DRAINING;
				return Drain_Step(pack_state, REQ_DRAINING, out_state);
			} else {
				if (Charge_GetMode() == CHRG_OFF) {
					mode = IDLE;
					return ERROR_NONE;
				}
				return Charge_Step(pack_state, REQ_IDLE, out_state);
			}
		} else {
			// Keep Draining
			return Drain_Step(pack_state, REQ_NONE, out_state);
		}
	} else {
		return ERROR_INCOMPATIBLE_MODE;
	}

	return ERROR_NONE;
}

MODE_T SSM_GetMode(void) {
	return mode;
}


