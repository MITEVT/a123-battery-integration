#include "charge.h"

static CHARGING_MODE_T mode = CHRG_INIT;

CHARGING_STATUS_T Charge_Step(PACK_STATE pack_state, MODE_T req_mode) {
	switch (req_mode) {
		case CHARGING:
			// Do nothing
			break;
		case DRAINING:
			// Move to exit state
			mode = CHRG_INIT;
			return CHRG_EXIT;
			break;
		case IDLE:
			// Move to exit state
			mode = CHRG_INIT;
			return CHRG_EXIT;
			break;
	}

	switch(mode) {
		case CHRG_INIT:
			// Start charging
			break;
		case CHRG_CHARGING:
			// Check that you can keep charging
			break;
		case CHRG_BALANCING:
			// Check that you can keep balancing
			break;
		case CHRG_EXIT:
			// Check that contactors have opened
			break;
	}
}

CHARGING_MODE_T Charge_GetMode() {
	return mode;
}