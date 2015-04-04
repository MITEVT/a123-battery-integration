#include "charge.h"

static CHARGING_MODE_T mode = CHRG_INIT;

CHARGING_STATUS_T Charge_Step(PACK_STATE *pack_state, MODE_T req_mode, OUTPUT_STATE *out_state) {
	switch (req_mode) {
		case CHARGING:
			// Do nothing
			break;
		case DRAINING:
			// Move to exit state
			mode = CHRG_EXIT;
			out_state->close_contactors = false;
			break;
		case IDLE:
			// Move to exit state
			mode = CHRG_EXIT;
			out_state->close_contactors = false;
			break;
	}

	switch(mode) {
		case CHRG_INIT:
			// Start charging
			if (!pack_state->contactors_closed) {
				out_state->close_contactors = true;
			} else {
				mode = CHARGING;
			}
			break;
		case CHRG_CHARGING:
			// Check that you can keep charging
			break;
		case CHRG_BALANCING:
			// Check that you can keep balancing
			break;
		case CHRG_EXIT:
			out_state->balance_voltage = BCM_BALANCE_OFF;
			if (!pack_state->contactors_closed) {
				mode = CHRG_INIT;
				return CHRG_EXITED;
			}
			break;
	}

	return CHRG_OK;
}

CHARGING_MODE_T Charge_GetMode() {
	return mode;
}