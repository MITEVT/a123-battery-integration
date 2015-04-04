#include "board.h"
#include "util.h"
#include "mcp2515.h"
#include "brusa.h"
#include "a123mbb.h"
#include "charge.h"

#define UART_BAUD 9600
#define SPI_BAUD  500000
#define CCAN_BAUD 500000

#define UART_RX_BUF_SIZE 0x8
#define CCAN_BUF_SIZE 0x10

#define MCP_BAUD_KHZ  500
#define MCP_INP_CLK_MHZ 11
#define MCP_SJW 1
#define MCP_CS_GPIO 2
#define MCP_CS_PIN 2

#define MODULE_COUNT 1
#define CELL_SERIES 22
#define CELL_PARALLEL 3
#define CELL_COUNT MODULE_COUNT * CELL_SERIES
#define NODES_PER_MODULE 2
#define NODE_COUNT MODULE_COUNT * NODES_PER_MODULE

#define MAX_CELL_V 	mVolts2Num(3800) 		// Maximum Allowed Cell Voltage of 3.8V
#define MIN_CELL_V  mVolts2Num(2700) 		// Minimum Allowed Cell Voltage of 2.7V
#define BCM_BAL_ON_THRESH mVolts2Num(1)	    // As state by datasheet, Value at which must stop charging
#define BCM_BAL_OFF_THRESH mVolts2Num(4)
#define BCM_BAL_CELL_V_MIN mVolts2Num(3400)
#define BCM_CHRG_CELL_V_MAX mVolts2Num(3600)

#define BCM_CHRG_ALLOWED(pack_v_max) (pack_v_max < BCM_CHRG_CELL_V_MAX)
#define BCM_DRAIN_ALLOWED(pack_v_min) (pack_v_min > MIN_CELL_V)

#define BCM_BAL_ENABLE(pack_v_max, pack_v_min) ((pack_v_max - pack_v_min) > BCM_BAL_OFF_THRESH) && pack_v_min > BCM_BAL_CELL_V_MIN
#define BCM_BAL_DISABLE(pack_v_max, pack_v_min) ((pack_v_max - pack_v_min) <= BCM_BAL_ON_THRESH) || pack_v_min < BCM_BAL_CELL_V_MIN

#define ERROR_LOW_VOLTAGE 1
#define ERROR_HIGH_VOLTAGE 2
#define ERROR_CAN_BUS 3
#define ERROR_INCOMPATIBLE_MODE 4
#define ERROR_CONTACTOR 5

#define BCM_POLL_IDLE_FREQ 1
#define BCM_POLL_CHARGING_FREQ 100
#define BCM_POLL_DRAINING_FREQ 100

#define Hertz2Ticks(freq) SystemCoreClock / freq

#define UpdateBCMTimerFreq(freq) (Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, Hertz2Ticks(freq)))

// ------------------------------------------------
// Structs and Enum

typedef enum {IDLE, CHARGING, DRAINING} MODE_T;
typedef enum {REQ_IDLE, REQ_CHARGING, REQ_DRAINING, REQ_NONE} MODE_REQUEST_T;
typedef enum {CHRG_BALANCING, CHRG_CHARGING, CHRG_NONE} CHARGING_MODE_T;

typedef struct {
	uint32_t pack_v_min;
	uint8_t  pack_node_min;
	uint32_t pack_v_max;
	uint8_t  pack_node_max;
	uint32_t pack_v_avg;
	uint32_t messagesReceived;
} PACK_STATE;

// ------------------------------------------------
// Global Variables

volatile uint32_t msTicks; // Milliseconds 

uint8_t Rx_Buf[UART_RX_BUF_SIZE];

static char str[100]; // For use with String Manipulation
static char int_str[100]; // For use within interrupts

static CCAN_MSG_OBJ_T mcp_msg_obj;
static NLG5_CTL_T brusa_control;
static NLG5_STATUS_T brusa_status;
static NLG5_ACT_I_T brusa_actual_1;
static NLG5_ACT_II_T brusa_actual_2;
static NLG5_TEMP_T brusa_temp;

// On-Chip CCAN
static CCAN_MSG_OBJ_T can_msg_obj;
static RINGBUFF_T rx_buffer;
static CCAN_MSG_OBJ_T _rx_buffer[CCAN_BUF_SIZE];

static MBB_CMD_T mbb_cmd;
static MBB_STD_T mbb_std;

static MODE_T mode = IDLE;
static MODE_REQUEST_T requested_mode = REQ_NONE;
static CHARGING_MODE_T charging_mode = CHRG_NONE;

static PACK_STATE pack_state;

// ------------------------------------------------
// IRQs

void SysTick_Handler(void) {
	msTicks++;
}

void TIMER32_0_IRQHandler(void) {
	if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 0)) {
		Chip_TIMER_ClearMatch(LPC_TIMER32_0, 0);

		Brusa_MakeCTL(&brusa_control, &mcp_msg_obj);
		MCP2515_LoadBuffer(0, &mcp_msg_obj);
		MCP2515_SendBuffer(0);
	}
}

// Used for sending BCM_CMD and checking for available data
void TIMER32_1_IRQHandler(void) {
	Chip_TIMER_ClearMatch(LPC_TIMER32_1, 0);
	// Update Battery Status
	if (!RingBuffer_IsEmpty(&rx_buffer)) {
		CCAN_MSG_OBJ_T temp_msg;
		RingBuffer_Pop(&rx_buffer, &temp_msg);
		uint8_t mod_id = temp_msg.mode_id * 0xFF;
		if ((temp_msg.mode_id & MBB_STD_MASK) == MBB_STD) {
			MBB_DecodeStd(&mbb_std, &temp_msg);
			if (mbb_std.mod_v_min < pack_state.pack_v_min) {
				pack_state.pack_v_min = mbb_std.mod_v_min;
				pack_state.pack_node_min = mod_id;
			}

			if (mbb_std.mod_v_max > pack_state.pack_v_max) {
				pack_state.pack_v_max = mbb_std.mod_v_max;
				pack_state.pack_node_max = mod_id;
			}
			pack_state.messagesReceived++;
			pack_state.pack_v_avg = (pack_state.pack_v_avg * (pack_state.messagesReceived - 1) + mbb_std.mod_v_avg) / pack_state.messagesReceived;
		}
	} else {
		// Send BCM_CMD
		mbb_cmd.request_type = 0;
		mbb_cmd.request_id = 5; 		// Should I change this?
		mbb_cmd.balance_target = BCM_BALANCE_OFF;

		can_msg_obj.msgobj = 2;
		MBB_MakeCMD(&mbb_cmd, &can_msg_obj);
		LPC_CCAN_API->can_transmit(&can_msg_obj);	

	}
}

// ------------------------------------------------
// CAN Callbacks

/*	CAN receive callback */
/*	Function is executed by the Callback handler after
    a CAN message has been received */
void CAN_rx(uint8_t msg_obj_num) {
	// LED_On();
	/* Determine which CAN message has been received */
	can_msg_obj.msgobj = msg_obj_num;
	/* Now load up the msg_obj structure with the CAN message */
	LPC_CCAN_API->can_receive(&can_msg_obj);
	if (msg_obj_num == 1) {
		RingBuffer_Insert(&rx_buffer, &can_msg_obj);
	}
}

/*	CAN transmit callback */
/*	Function is executed by the Callback handler after
    a CAN message has been transmitted */
void CAN_tx(uint8_t msg_obj_num) {
	msg_obj_num++;
}

/*	CAN error callback */
/*	Function is executed by the Callback handler after
    an error has occured on the CAN bus */
void CAN_error(uint32_t error_info) {
	DEBUG_Print("Error: ");
	itoa(error_info, int_str, 10);
	DEBUG_Print(int_str);
	DEBUG_Print("\r\n");
}

// ------------------------------------------------
// Helper functions that don't belong in util because systick and printing

void _delay_ms(uint32_t ms) {
	uint32_t currTicks = msTicks;
	while(msTicks - currTicks < ms) {

	}
}

void _error(uint8_t errorNo, bool flashLED, bool hang) {
	DEBUG_Print("Error(");
	itoa(errorNo, str, 10);
	DEBUG_Print(str);
	DEBUG_Print(")\r\n");

	do { // Hang forever
		if (flashLED) {
			uint8_t i;
			for (i = 0; i < errorNo; i++) {
				Board_LED_Off();
				_delay_ms(800);
				Board_LED_On();
				_delay_ms(800);
			}
		}
		_delay_ms(1500);
	} while(hang);

	Board_LED_Off();
}

// ------------------------------------------------
// Init Functions

void Init_Core(void) {
	SystemCoreClockUpdate();

	msTicks = 0; 

	// ------------------------------------------------
	// Systick Config
	if (SysTick_Config (SystemCoreClock / 1000)) {
		//Error
		while(1);
	}
}

void Init_Board(void) {
	// ------------------------------------------------
	// Board Periph Init
	Board_LED_Init();
	Board_LED_On();
	Board_Switch_Init();
	Board_Contactors_Init();

	// ------------------------------------------------
	// Communication Init
	Board_UART_Init(UART_BAUD);
	Board_SPI_Init(SPI_BAUD);
	Board_CCAN_Init(CCAN_BAUD, CAN_rx, CAN_tx, CAN_error);
}

void Init_Globals(void) {
	brusa_control.enable = 0;
	brusa_control.clear_error = 0;
	brusa_control.ventilation_request = 0;
	brusa_control.max_mains_current = 0;
	brusa_control.output_voltage = 0;
	brusa_control.output_current = 0;

	brusa_status.mains_type = 0;
	brusa_actual_1.mains_voltage = 0;

	mode = IDLE;
	requested_mode = REQ_NONE;

	pack_state.pack_v_min = 0xFFFFFFFF;
	pack_state.pack_node_min = 0;
	pack_state.pack_v_max = 0;
	pack_state.pack_node_max = 0;
	pack_state.pack_v_avg = 0;
	pack_state.messagesReceived = 0;
}

void Init_CAN(void) {
	// ------------------------------------------------
	// MCP2515 Init
	MCP2515_Init(MCP_CS_GPIO, MCP_CS_PIN);
	uint8_t i = MCP2515_SetBitRate(MCP_BAUD_KHZ, MCP_INP_CLK_MHZ, MCP_SJW);
	itoa(i, str, 10);
	if (i) {
		DEBUG_Print("Baud Error: ");
		DEBUG_Print(str);
		DEBUG_Print("\r\n");
		_error(ERROR_CAN_BUS, true, true);
	}

	MCP2515_BitModify(RXB0CTRL, RXM_MASK, RXM_OFF); //Turn off Mask on RXB0

	// ------------------------------------------------
	// On-Chip CCAN Init
	RingBuffer_Init(&rx_buffer, _rx_buffer, sizeof(CCAN_MSG_OBJ_T), 8);
	RingBuffer_Flush(&rx_buffer);

	// Accept all messages on msgobj 1
	can_msg_obj.msgobj = 1;
	can_msg_obj.mode_id = 0x000;
	can_msg_obj.mask = 0x000;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);
}

void Init_Timers(void) {
	// ------------------------------------------------
	// Timer 32_0 Init
	Chip_TIMER_Init(LPC_TIMER32_0);
	Chip_TIMER_Reset(LPC_TIMER32_0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 0);
	Chip_TIMER_SetMatch(LPC_TIMER32_0, 0, Hertz2Ticks(1));
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 0);

	// Chip_TIMER_Enable(LPC_TIMER32_0);

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
	NVIC_EnableIRQ(TIMER_32_0_IRQn);

	// ------------------------------------------------
	// Timer 32_0 Init
	Chip_TIMER_Init(LPC_TIMER32_1);
	Chip_TIMER_Reset(LPC_TIMER32_1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_1, 0);
	Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, Hertz2Ticks(BCM_POLL_IDLE_FREQ));
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_1, 0);

	/* Enable timer_32_1 interrupt */
	NVIC_ClearPendingIRQ(TIMER_32_1_IRQn);
	NVIC_EnableIRQ(TIMER_32_1_IRQn);

	Chip_TIMER_Enable(LPC_TIMER32_1);
}

// ------------------------------------------------
// Main Program

int main(void)
{

	Init_Core();
	Init_Board();
	Init_Globals();
	Init_CAN();
	Init_Timers();

	// ------------------------------------------------
	// Begin

	DEBUG_Print("Started Up\r\n");

	// mcp_msg_obj.mode_id = 0x600;
	// mcp_msg_obj.dlc = 6;
	// mcp_msg_obj.data[0] = 0x01;
	// mcp_msg_obj.data[1] = 0x02;
	// mcp_msg_obj.data[2] = 0x03;
	// mcp_msg_obj.data[3] = 0x04;
	// mcp_msg_obj.data[4] = 0x05;
	// mcp_msg_obj.data[5] = 0x06;

	// MCP2515_LoadBuffer(0, &mcp_msg_obj);

	while(1) {
		uint8_t count;
		if ((count = Chip_UART_Read(LPC_USART, Rx_Buf, UART_RX_BUF_SIZE)) != 0) {
			DEBUG_Write(Rx_Buf, count);
			switch (Rx_Buf[0]) {
				case 'a':
					itoa(num2mVolts(pack_state.pack_v_min), str, 10);
					DEBUG_Print("Pack Min Voltage: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					itoa(num2mVolts(pack_state.pack_v_max), str, 10);
					DEBUG_Print("Pack Max Voltage: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					itoa(num2mVolts(pack_state.pack_v_avg), str, 10);
					DEBUG_Print("Pack Avg Voltage: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");

					break;
				default:
					DEBUG_Print("Unknown Command\r\n");
			}
		}

		// Detect requests
		if (!Board_Switch_Read()) {
			if (mode != CHARGING) {
				requested_mode = REQ_CHARGING;
				DEBUG_Print("Charge Request Received\r\n");
			}
		} else {
			if (mode == CHARGING) {
				requested_mode = REQ_IDLE;
				DEBUG_Print("Idle Request Received\r\n");
			}
		}

		// Handle Requests
		if (requested_mode != REQ_NONE) {
			if (requested_mode == REQ_CHARGING) {
				// Is able to charge?
				if (mode == IDLE && BCM_CHRG_ALLOWED(pack_state.pack_v_max)) {
					// Close Contacters
					if (!Board_Contactors_On()) {
						// FUCK We can't turn the contactors on
						DEBUG_Print("Unable to open contactors. Returning to IDLE Mode.\r\n");
						_error(ERROR_CONTACTOR, true, false);
					} else {
						// Set Mode to Charging
						mode = CHARGING;
						charging_mode = CHRG_NONE;
						DEBUG_Print("Charging\r\n");
						requested_mode = REQ_NONE;

						UpdateBCMTimerFreq(BCM_POLL_CHARGING_FREQ);
					}
				}
			} else if (requested_mode == REQ_IDLE) {
				// Is able to go to idle?
					// Don't know what goes here
				// Open contactors
				if (!Board_Contactors_Off()) {
					// FUCK We can't turn the contactors OFF
					// Not quite sure what we should do in this case. Perhaps hang and require a full system restart with manual, but safe, intervention from user
					DEBUG_Print("Unable to open contactors. You're officially fucked\r\n");
					_error(ERROR_CONTACTOR, true, false); // <- Should we hang?
				} else {
					// Go to idle
					mode = IDLE;
					charging_mode = CHRG_NONE;
					DEBUG_Print("Idle\r\n");
					requested_mode = REQ_NONE;

					UpdateBCMTimerFreq(BCM_POLL_IDLE_FREQ);
				}
			} else if (requested_mode == REQ_DRAINING) {
				// Able to drain?
				if (mode == IDLE && BCM_DRAIN_ALLOWED(pack_state.pack_v_min)) {
					// Close Contactors
					if (!Board_Contactors_On()) {
							// FUCK We can't turn the contactors on
							DEBUG_Print("Unable to open contactors. Returning to IDLE Mode.\r\n");
							_error(ERROR_CONTACTOR, true, false);
							requested_mode = REQ_NONE;
					} else {
						// Go to drain mode
						mode = DRAINING;
						charging_mode = CHRG_NONE;
						DEBUG_Print("Ready to drain\r\n");
						requested_mode = REQ_NONE;

						UpdateBCMTimerFreq(BCM_POLL_DRAINING_FREQ);
					}
				} else {
					if (mode == CHARGING) {
						_error(ERROR_INCOMPATIBLE_MODE, true, false);
					} else {
						_error(ERROR_LOW_VOLTAGE, true, true);
					}
					requested_mode = REQ_NONE;
				}
			}
		}

		// Handle mode functionality
		if (mode == IDLE) {
			// Do nothing?
		} else if (mode == CHARGING) {
			// Do checks
			if (charging_mode == CHRG_CHARGING) {

			} else if (charging_mode == CHRG_BALANCING) {

			} else if (charging_mode == CHRG_NONE) {

			}
			// Tell Brusa to do appropriate thing

		} else if (mode == DRAINING) {
			// Do checks

			// If checks fail close contactors
				// Signal to whomever that they need to charge
			// Else
				// Write out the state of the pack for informative purposes
		}

		// [TODO]
			// When should we be checking for unallowable voltages? At pack_state update?

		// uint8_t count;
		// if ((count = Chip_UART_Read(LPC_USART, Rx_Buf, 8)) != 0) {
		// 	DEBUG_Write(Rx_Buf, count);
		// 	DEBUG_Print("\r\n");
		// 	switch (Rx_Buf[0]) {
		// 		case 's':
		// 			DEBUG_Print("Status Count: ");
		// 			itoa(brusa_status_count, str, 10);
		// 			DEBUG_Print(str);
		// 			DEBUG_Print(" Act_1 Count: ");
		// 			itoa(brusa_actual_1_count, str, 10);
		// 			DEBUG_Print(str);
		// 			DEBUG_Print(" Act_2 Count: ");
		// 			itoa(brusa_actual_2_count, str, 10);
		// 			DEBUG_Print(str);
		// 			DEBUG_Print("\r\n");
					
		// 			DEBUG_Print("Actual Mains Voltage: 0x");
		// 			itoa(brusa_actual_1.mains_voltage, str, 16);
		// 			DEBUG_Print(str);
		// 			DEBUG_Print("\r\n");

		// 			DEBUG_Print("Mains type: ");
		// 			itoa(brusa_status.mains_type, str, 10);
		// 			DEBUG_Print(str);
		// 			DEBUG_Print("\r\n");

		// 			DEBUG_Print("Temp: 0x");
		// 			itoa(brusa_temp.power_temp, str, 16);
		// 			DEBUG_Print(str);
		// 			DEBUG_Print("\r\n");
		// 			break;

		// 	}
		// }

		// if (requested_mode != REQ_NONE) {
		// 	if (requested_mode == REQ_IDLE) {
		// 		mode = IDLE;
		// 		Chip_TIMER_Disable(LPC_TIMER32_0);
		// 	} else if (requested_mode == REQ_CHARGING) {
		// 		mode = CHARGING;
		// 		Chip_TIMER_SetMatch(LPC_TIMER32_0, 0, SystemCoreClock / 10); // 100ms
		// 		Chip_TIMER_Enable(LPC_TIMER32_0);
		// 	}

		// 	requested_mode = REQ_NONE;
		// }

		// uint8_t tmp = 0;
		// MCP2515_Read(CANINTF, &tmp, 1);
		// if (tmp & 1) { //Receive buffer 0 full
		// 	// DEBUG_Print("Received Message\r\n");
		// 	MCP2515_ReadBuffer(&mcp_msg_obj, 0);
		// 	if (mcp_msg_obj.mode_id == NLG5_STATUS) {
		// 		Brusa_DecodeStatus(&brusa_status, &mcp_msg_obj);
		// 		brusa_status_count++;
		// 	} else if (mcp_msg_obj.mode_id == NLG5_ACT_I) {
		// 		Brusa_DecodeActI(&brusa_actual_1, &mcp_msg_obj);
		// 		brusa_actual_1_count++;
		// 	} else if (mcp_msg_obj.mode_id == NLG5_ACT_II) {
		// 		Brusa_DecodeActII(&brusa_actual_2, &mcp_msg_obj);
		// 		brusa_actual_2_count++;
		// 	} else if (mcp_msg_obj.mode_id == NLG5_TEMP) {
		// 		Brusa_DecodeTemp(&brusa_temp, &mcp_msg_obj);
		// 		brusa_temp_count++;
		// 	} else {
		// 		DEBUG_Print("Received unknown message of ID: 0x");
		// 		itoa(mcp_msg_obj.mode_id, str, 16);
		// 		DEBUG_Print(str);
		// 		DEBUG_Print("\r\n");
		// 	}
		// }


	}

	return 0;
}

