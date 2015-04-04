//Progresses state of DRAIN state machine
//	If function is called, current mode must be DRAINING
//	next_mode is the next requested mode

//INIT

#include "drain.h"

DRAIN_STATUS_T Drain_Step(PACK_STATE* pack_state, MODE_T next_mode, OUTPUT_STATE* os){
	switch(next_mode){
		case IDLE:
			break;

		case CHARGING:
			//Check if can continue drain
			//Return output state request
			break;

		case DRAINING:
			//Check if can continue drain
			//Return output state request, contactor = close
			break;
	}
	return DRAIN_OK;
}


DRAIN_MODE_T Drain_GetMode(){
	return DRAIN_EXIT;
}
