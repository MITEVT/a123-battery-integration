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
				mode = CHRG_CHARGING;
			}
			break;
		case CHRG_CHARGING:
			// Check that you can keep charging
			if (BCM_CHRG_ALLOWED(pack_state->pack_v_max)) {
				// Set proper voltage and current on out_state
			} else {
				// Need to stop charging
				// Tell brusa to stop
				out_state->brusa_voltage = 0;
				out_state->brusa_current = 0;
				if (BCM_BAL_ENABLE(pack_state->pack_v_max, pack_state->pack_v_min)) {
					// Move to balancing
					mode = CHRG_BALANCING;
				} else {
					mode = CHRG_INIT;
					return CHRG_ERROR;
				}
			}
			break;
		case CHRG_BALANCING:
			// Check that you can keep balancing
			if (BCM_BAL_DISABLE(pack_state->pack_v_max, pack_state->pack_v_min)) {
				// Need to stop balancing
				out_state->balance_voltage = BCM_BALANCE_OFF;
				if (BCM_CHRG_ALLOWED(pack_state->pack_v_max)) {
					mode = CHRG_CHARGING;
				}
			} else {
				// Keep balancing
				out_state->balance_voltage = pack_state->pack_v_min;
			}
			break;
		case CHRG_EXIT:
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