#include "drain.h"

static DRAIN_MODE_T mode = DRAIN_OFF;

//Progresses state of DRAIN state machine
ERROR_T Drain_Step(PACK_STATE_T* pack_state, MODE_REQUEST_T req_mode, OUTPUT_STATE_T* out_state){
	// switch(req_mode){
	// 	case IDLE:
	// 		os->close_contactors = false;
	// 		mode = DRAIN_EXIT;
	// 		return DRAIN_OK;
	// 		break;

	// 	case CHARGING:
	// 		os->close_contactors = false;
	// 		mode = DRAIN_EXIT;
	// 		return DRAIN_OK;
	// 		break;

	// 	case DRAINING:
	// 		//Is battery in state to drain?
	// 		if(BCM_DRAIN_ALLOWED(pack_state->pack_v_min)) {
	// 			os->close_contactors = true;
	// 			mode = DRAIN_DRAINING;
	// 			return DRAIN_OK;
	// 		} else {
	// 			os->close_contactors = false;
	// 			return DRAIN_ERROR;
	// 		}
	// 		break;
	// }

	switch (req_mode) {
		case REQ_CHARGING:
			// If starting from not charging, go to init, else stay where you are
			mode = DRAIN_OFF;
			out_state->close_contactors = false;
			break;
		case REQ_DRAINING:
			// Move to exit state
			mode = (mode == DRAIN_OFF) ? DRAIN_INIT : mode;
			break;
		case REQ_IDLE:
			// Move to exit state
			mode = DRAIN_OFF;
			out_state->close_contactors = false;
			break;
		case REQ_NONE:
			// Chill
			break;
	}
	out_state->brusa_mVolts = 0;
	out_state->brusa_cAmps = 0;
	out_state->balance_mVolts = BCM_BALANCE_OFF;
	switch(mode) {
		case DRAIN_OFF:
			out_state->close_contactors = false;
			break;
		case DRAIN_INIT:
			// Start Draining
			if (!pack_state->contactors_closed) {
				out_state->close_contactors = true;
			} else {
				mode = DRAIN_DRAINING;
			}
			break;
		case DRAIN_DRAINING:
			// Monitor voltages
			// Move to Drain_Done if illegal voltage
			break;
		case DRAIN_DONE:
			out_state->close_contactors = false;
			break;
	}

	return ERROR_NONE;
}


DRAIN_MODE_T Drain_GetMode(void){
	return mode;
}
