#include "balance.h"

static BALANCING_MODE_T mode = BAL_OFF;

static bool balancing;

ERROR_T Balance_Step(PACK_STATE_T *pack_state, MODE_REQUEST_T req_mode, OUTPUT_STATE_T *out_state) {
	switch (req_mode) {
		case REQ_BALANCING:
			mode = (mode == BAL_OFF) ? BAL_INIT : mode;
			break;
		case REQ_CHARGING:
			// Move to exit state
			mode = BAL_OFF;
			break;
		case REQ_DRAINING:
			// Move to exit state
			mode = BAL_OFF;
			break;
		case REQ_IDLE:
			// Move to exit state
			mode = BAL_OFF;
			break;
		case REQ_NONE:
			// Chill
			break;
	}
handler:
	out_state->close_contactors = false;
	out_state->brusa_clear_latch = false;
	out_state->brusa_output = false;
	out_state->brusa_mVolts = 0;
	out_state->brusa_cAmps = 0;
	switch(mode) {
		case BAL_OFF:
			out_state->balance = false;
			break;
		case BAL_INIT:
			// Start balancing
			out_state->balance = false;

			mode = BAL_ON;
			goto handler;
			break;
		case BAL_ON:

			if (!balancing && BAL_ENABLE(pack_state->pack_max_mVolts, pack_state->pack_min_mVolts)) {
				balancing = true;
			} else if (balancing && 
						BAL_DISABLE(pack_state->pack_max_mVolts, 
										pack_state->pack_min_mVolts)) {
				balancing = false;
			}

			if (balancing) {
				out_state->balance = true;
				out_state->balance_mVolts = pack_state->pack_min_mVolts;
			} else {
				out_state->balance = false;
				out_state->balance_mVolts = BCM_BALANCE_OFF;
			}
			break;
		case BAL_DONE:
			out_state->close_contactors = false;
			out_state->balance = false;
			break;
	}

	return ERROR_NONE;
}

BALANCING_MODE_T Balance_GetMode() {
	return mode;
}