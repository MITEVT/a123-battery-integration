//Progresses state of DRAIN state machine
//	If function is called, current mode must be DRAINING
//	next_mode is the next requested mode

#include "drain.h"

static DRAIN_MODE_T mode = DRAIN_INIT;

DRAIN_STATUS_T Drain_Step(PACK_STATE* pack_state, MODE_T req_mode, OUTPUT_STATE* os){
	switch(req_mode){
		case IDLE:
			os->close_contactors = false;
			mode = DRAIN_EXIT;
			return DRAIN_OK;
			break;

		case CHARGING:
			os->close_contactors = false;
			mode = DRAIN_EXIT;
			return DRAIN_OK;
			break;

		case DRAINING:
			//Is battery in state to drain?
			if(BCM_DRAIN_ALLOWED(pack_state->pack_v_min)) {
				os->close_contactors = true;
				mode = DRAIN_DRAINING;
				return DRAIN_OK;
			} else {
				os->close_contactors = false;
				return DRAIN_ERROR;
			}
			break;
	}
}


DRAIN_MODE_T Drain_GetMode(void){
	return mode;
}
