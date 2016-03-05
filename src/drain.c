#include "drain.h"

static DRAIN_MODE_T mode = DRAIN_OFF;
static uint32_t minimum_cell_mVolts;


void Drain_Config(DRAINING_CONFIG_T *config) {
	minimum_cell_mVolts = config->min_cell_mVolts;
	mode = DRAIN_OFF;
}

//Progresses state of DRAIN state machine
ERROR_T Drain_Step(PACK_STATE_T* pack_state, MODE_REQUEST_T req_mode, OUTPUT_STATE_T* out_state) {
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
		case REQ_BALANCING:
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
			if (pack_state->pack_min_mVolts <= minimum_cell_mVolts) {
				mode = DRAIN_DONE;
				out_state->close_contactors = false;
				return ERROR_LOW_VOLTAGE;
			}
			// Move to Drain_Done if illegal voltage
			break;
		case DRAIN_DONE:
			out_state->close_contactors = false;
			break;
	}

	return ERROR_NONE;
}


DRAIN_MODE_T Drain_GetMode(void) {
	return mode;
}
