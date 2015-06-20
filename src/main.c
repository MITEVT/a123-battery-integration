#include "board.h"
#include <string.h>

#define UART_BAUD 57600
#define SPI_BAUD  500000
#define CCAN_BAUD 500000

#define UART_RX_BUF_SIZE 0x8
#define CCAN_BUF_SIZE 0x10

#define MCP_BAUD_KHZ  500
#define MCP_INP_CLK_MHZ 11
#define MCP_SJW 1
#define MCP_CS_GPIO 2
#define MCP_CS_PIN 2
#define MCP_INT_GPIO 0
#define MCP_INT_PIN 2

#define MODULE_COUNT 1
#define CELL_SERIES 22
#define CELL_PARALLEL 3
#define CELL_COUNT MODULE_COUNT * CELL_SERIES
#define NODES_PER_MODULE 2
#define NODE_COUNT MODULE_COUNT * NODES_PER_MODULE

#define BCM_POLL_IDLE_FREQ 1
#define BCM_POLL_CHARGING_FREQ 100
#define BCM_POLL_DRAINING_FREQ 100

#define BRUSA_MAX_MAINS_CAMPS 1500

#define Hertz2Ticks(freq) SystemCoreClock / freq

#define UpdateBCMTimerFreq(freq) (Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, Hertz2Ticks(freq)))

// ------------------------------------------------
// Global Static Variables

static volatile uint64_t msTicks; // Milliseconds 

uint8_t Rx_Buf[UART_RX_BUF_SIZE];

static char str[100]; // For use with String Manipulation
static char int_str[100];

static CCAN_MSG_OBJ_T mcp_msg_obj;
static uint64_t brusa_error_message;
static NLG5_CTL_T brusa_control;
static NLG5_STATUS_T brusa_status;
static NLG5_ACT_I_T brusa_actual_1;
static NLG5_ACT_II_T brusa_actual_2;
static NLG5_TEMP_T brusa_temp;
static volatile uint32_t brusa_control_count = 0;
static volatile bool brusa_message_send = false;

static MODE_T mode;
// On-Chip CCAN
static CCAN_MSG_OBJ_T can_msg_obj;
static RINGBUFF_T rx_buffer;
static CCAN_MSG_OBJ_T _rx_buffer[CCAN_BUF_SIZE];

static MBB_CMD_T mbb_cmd;
static MBB_STD_T mbb_std;

static PACK_STATE_T pack_state;
static OUTPUT_STATE_T out_state;

static MBB_EXT_T mbb_ext_1;
static MBB_EXT_T mbb_ext_2;


// ------------------------------------------------
// Helper functions that don't belong in util because systick and printing

void _delay_ms(uint32_t ms) {
	uint32_t currTicks = msTicks;
	while(msTicks - currTicks < ms) {

	}
}

void _error(ERROR_T errorNo, bool flashLED, bool hang) {
	DEBUG_Print("Error(");
	itoa(errorNo, str, 10);
	DEBUG_Print(str);
	DEBUG_Print(")\r\n");

	do { // Hang forevers
		if (flashLED) {
			uint8_t i;
			for (i = 0; i < errorNo; i++) {
				Board_LED_Off();
				_delay_ms(500);
				Board_LED_On();
				_delay_ms(500);
			}
		}
		_delay_ms(1500);
		Board_LED_Off();
	} while(hang);

	Board_LED_On();
}

// [TODO] Move to Brusa Library
void decodeBrusa(CCAN_MSG_OBJ_T *msg) {
	if (msg->mode_id == NLG5_STATUS) {
		Brusa_DecodeStatus(&brusa_status, msg);
		// brusa_messages_received[0] = true;
		// DEBUG_Println(">0");
	} else if (msg->mode_id == NLG5_TEMP) {
		Brusa_DecodeTemp(&brusa_temp, msg);
		// brusa_messages_received[3] = true;
		// DEBUG_Println(">3");
	} else if (msg->mode_id == NLG5_ACT_I) {
		Brusa_DecodeActI(&brusa_actual_1, msg);
		// DEBUG_Print("Actual Out Voltage: ");
		// itoa(brusa_actual_1.output_mVolts, str, 10);
		// DEBUG_Println(str);

		// DEBUG_Print("Actual Out Current: ");
		// itoa(brusa_actual_1.output_cAmps, str, 10);
		// DEBUG_Println(str);
		// brusa_messages_received[1] = true;
		// DEBUG_Println(">1");
	} else if (msg->mode_id == NLG5_ACT_II) {
		Brusa_DecodeActII(&brusa_actual_2, msg);
		// brusa_messages_received[2] = true;
		// DEBUG_Println(">2");
	} else if (msg->mode_id == NLG5_ERR) {
		brusa_error_message = 0;
		msg->data[4] &= 0xF0;
		int i;
		for (i = 0; i < msg->dlc; i++) {
			brusa_error_message |= msg->data[i] << (8*i);
		}
		// brusa_messages_received[4] = true;
		// DEBUG_Println(">4");
	} else {
		DEBUG_Print("!!U");
	}
}

// ------------------------------------------------
// IRQs

void SysTick_Handler(void) {
	msTicks++;
	pack_state.msTicks = msTicks;
}

void TIMER32_0_IRQHandler(void) {
	if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 0)) {
		Chip_TIMER_ClearMatch(LPC_TIMER32_0, 0);

		brusa_message_send = true;
	}
}

static volatile bool newMessage = true;
static uint8_t arbitraryCount = 0;
// Used for sending BCM_CMD and checking for available data
void TIMER32_1_IRQHandler(void) {
	Chip_TIMER_ClearMatch(LPC_TIMER32_1, 0);
	// Update Battery Status
	if (RingBuffer_IsEmpty(&rx_buffer)) {
		// Send BCM_CMD
		mbb_cmd.request_type = BCM_REQUEST_TYPE_STD;
		mbb_cmd.request_id = 5; 		// Should I change this?
		mbb_cmd.balance = out_state.balance;
		mbb_cmd.balance_target_mVolts = out_state.balance_mVolts;

		can_msg_obj.msgobj = 7;
		MBB_MakeCMD(&mbb_cmd, &can_msg_obj);
		LPC_CCAN_API->can_transmit(&can_msg_obj);	

		newMessage = true;
		if (arbitraryCount++ == 4) {
			arbitraryCount = 0;
			mbb_cmd.request_type = BCM_REQUEST_TYPE_EXT;

			can_msg_obj.msgobj = 6;
			mbb_cmd.request_id = 6;
			MBB_MakeCMD(&mbb_cmd, &can_msg_obj);
			LPC_CCAN_API->can_transmit(&can_msg_obj);

		}
	}
}

// ------------------------------------------------
// CAN Callbacks

/*	CAN receive callback */
/*	Function is executed by the Callback handler after
    a CAN message has been received */
void CAN_rx(uint8_t msg_obj_num) {
	/* Determine which CAN message has been received */
	can_msg_obj.msgobj = msg_obj_num;
	/* Now load up the msg_obj structure with the CAN message */
	LPC_CCAN_API->can_receive(&can_msg_obj);
	RingBuffer_Insert(&rx_buffer, &can_msg_obj);
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

// NOTE: Because these callbacks are called by an interrupt routine
// 		CANNOT use anything with msTicks, including _error, so should set a f
void CAN_error(uint32_t error_info) {
	error_info++;
	DEBUG_Print("On-chip CAN Error\n\r");
	// _error(error_info, true, false);
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

void Init_SM(void) {

	CHARGING_CONFIG_T charge_config;
	charge_config.pack_s = 22;
	charge_config.pack_p = 3;
	charge_config.max_cell_mVolts = 3600;
	charge_config.cc_cell_mVolts = 3700;
	charge_config.cell_capacity_cAmpHours = 2000; 	// 20 Ahr
	charge_config.cell_mC_rating = 133; 			// .133 C

	DRAINING_CONFIG_T drain_config;
	drain_config.min_cell_mVolts = 2500; //Specs say 2000mV but lets play it safe for now

	Charge_Config(&charge_config);
	Drain_Config(&drain_config);
	SSM_Init();

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
	brusa_control.enable = 1;
	brusa_control.clear_error = 0;
	brusa_control.ventilation_request = 1;
	brusa_control.max_mains_cAmps = BRUSA_MAX_MAINS_CAMPS;
	brusa_control.output_mVolts = 0;
	brusa_control.output_cAmps = 0;

	pack_state.pack_min_mVolts = 0xFFFFFFFF;
	pack_state.pack_node_min = 0;
	pack_state.pack_max_mVolts = 0;
	pack_state.pack_node_max = 0;
	pack_state.pack_avg_mVolts = 0;
	pack_state.messagesReceived = 0;

	out_state.balance_mVolts = BCM_BALANCE_OFF;
	out_state.brusa_mVolts = 0;
	out_state.brusa_cAmps = 0;
	out_state.close_contactors = false;
	out_state.brusa_output = false;
	out_state.brusa_clear_latch = false;

	mbb_ext_1.id = 0xFF;
	mbb_ext_2.id = 0xFF;

	mbb_ext_1.bal = 0;
	mbb_ext_2.bal = 0;

	mode = IDLE;
}

void Init_CAN(void) {
	// ------------------------------------------------
	// MCP2515 Init
	MCP2515_Init(MCP_CS_GPIO, MCP_CS_PIN, MCP_INT_GPIO, MCP_INT_PIN);
	uint8_t i = MCP2515_SetBitRate(MCP_BAUD_KHZ, MCP_INP_CLK_MHZ, MCP_SJW);
	itoa(i, str, 10);
	if (i) {
		DEBUG_Print("Baud Error: ");
		DEBUG_Print(str);
		DEBUG_Print("\r\n");
		_error(ERROR_CAN_BUS, true, true);
	}

	Brusa_MakeCTL(&brusa_control, &mcp_msg_obj);
	MCP2515_LoadBuffer(0, &mcp_msg_obj);

	MCP2515_BitModify(RXB0CTRL, RXM_MASK, RXM_STD); //Turn RXB0 Mask
	MCP2515_BitModify(RXB1CTRL, RXM_MASK, RXM_STD);
	MCP2515_BitModify(RXF0SIDH, 0xFF, 0xC2);
	MCP2515_BitModify(RXF0SIDL, 0xE0, 0x00); // Message 610
	MCP2515_BitModify(RXF1SIDH, 0xFF, 0xC2);
	MCP2515_BitModify(RXF1SIDL, 0xE0, 0x80); // Message 614
	MCP2515_BitModify(RXM0SIDH, 0xFF, 0xFF);
	MCP2515_BitModify(RXM0SIDL, 0xE0, 0xE0); // Buffer 0 gets odd
	MCP2515_BitModify(RXF2SIDH, 0xFF, 0xC2);
	MCP2515_BitModify(RXF2SIDL, 0xE0, 0x20); // Message 611
	MCP2515_BitModify(RXF3SIDH, 0xFF, 0xC2);
	MCP2515_BitModify(RXF3SIDL, 0xE0, 0x60); // Message 613
	MCP2515_BitModify(RXF4SIDH, 0xFF, 0xC2);
	MCP2515_BitModify(RXF4SIDL, 0xE0, 0x20); // Message 611
	MCP2515_BitModify(RXF5SIDH, 0xFF, 0xC2);
	MCP2515_BitModify(RXF5SIDL, 0xE0, 0x20); // Message 611
	MCP2515_BitModify(RXM1SIDH, 0xFF, 0xFF);
	MCP2515_BitModify(RXM1SIDL, 0xE0, 0xE0); // Buffer 1 gets even

	MCP2515_Mode(MODE_NORMAL);

	// ------------------------------------------------
	// On-Chip CCAN Init
	RingBuffer_Init(&rx_buffer, _rx_buffer, sizeof(CCAN_MSG_OBJ_T), 8);
	RingBuffer_Flush(&rx_buffer);

	// Accept all messages on msgobj 1
	can_msg_obj.msgobj = 1;
	can_msg_obj.mode_id = 0x200;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);

	can_msg_obj.msgobj = 2;
	can_msg_obj.mode_id = 0x300;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);

	can_msg_obj.msgobj = 3;
	can_msg_obj.mode_id = 0x400;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);

	can_msg_obj.msgobj = 4;
	can_msg_obj.mode_id = 0x500;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);

	can_msg_obj.msgobj = 9;
	can_msg_obj.mode_id = 0x600;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);
}

void Init_Timers(void) {
	// ------------------------------------------------
	// Timer 32_0 Init
	Chip_TIMER_Init(LPC_TIMER32_0);
	Chip_TIMER_Reset(LPC_TIMER32_0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 0);
	Chip_TIMER_SetMatch(LPC_TIMER32_0, 0, Hertz2Ticks(10));
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

bool b = true;
uint8_t counttt = 0;

int main2(void) {
	Init_Core();
	Init_SM();
	Init_Board();
	Init_Globals();
	Init_CAN();
	Init_Timers();


	counttt = 0;
	brusa_control.output_mVolts = 0;
	brusa_control.output_cAmps = 0;
	brusa_control.ventilation_request = 1;
	Chip_TIMER_Enable(LPC_TIMER32_0);
	brusa_control_count = 0;

	DEBUG_Print("Timer Enabled");

	uint8_t rec = 0;
	uint8_t tec = 0;
	while(1) {

		// if (counttt == 20) {
		// 	counttt++;
		// 	DEBUG_Println("Starting this bitch");
		// 	brusa_control.output_mVolts = 73100;
		// 	brusa_control.output_cAmps = 100;
		// }

		uint8_t count;
		if ((count = Chip_UART_Read(LPC_USART, Rx_Buf, UART_RX_BUF_SIZE)) != 0) {
			switch (Rx_Buf[0]) {
				case 'a':				
					DEBUG_Print("Actual Mains Voltage: ");
					itoa(brusa_actual_1.mains_mVolts, str, 10);
					DEBUG_Print(str);
					DEBUG_Print("\r\n");

					DEBUG_Print("Mains type: ");
					itoa(brusa_actual_1.mains_cAmps, str, 10);
					DEBUG_Print(str);
					DEBUG_Print("\r\n");

					DEBUG_Print("Temp: 0x");
					itoa(brusa_temp.power_temp, str, 16);
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					break;
				case 'b':
					DEBUG_Print("Actual Out Voltage: ");
					itoa(brusa_actual_1.output_mVolts, str, 10);
					DEBUG_Println(str);

					DEBUG_Print("Actual Out Current: ");
					itoa(brusa_actual_1.output_cAmps, str, 10);
					DEBUG_Println(str);
					break;
				case 'e':
					MCP2515_Read(REC, &rec, 1);
					MCP2515_Read(TEC, &tec, 1);

					DEBUG_Print("REC: ");
					itoa(rec, str, 10);
					DEBUG_Println(str);

					DEBUG_Print("TEC: ");
					itoa(tec, str, 10);
					DEBUG_Println(str);
					break;
				case 'f':
					itoa(pack_state.pack_min_mVolts, str, 10);
					DEBUG_Print("Pack Min Voltage: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					itoa(pack_state.pack_max_mVolts, str, 10);
					DEBUG_Print("Pack Max Voltage: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					itoa(pack_state.pack_avg_mVolts, str, 10);
					DEBUG_Print("Pack Avg Voltage: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					itoa(pack_state.messagesReceived, str, 10);
					DEBUG_Print("Messages Received: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");

					break;
				case 'y':
					itoa(mbb_ext_1.bal, str, 2);
					DEBUG_Print("Mod 1: 0b");
					DEBUG_Println(str);
					itoa(mbb_ext_2.bal, str, 2);
					DEBUG_Print("Mod 2: 0b");
					DEBUG_Println(str);
					break;
				case 'z':
					for (count = 0; count < 12; count++) {
						itoa(mbb_ext_1.cell_mVolts[count], str, 10);
						DEBUG_Println(str);
					}

					for (count = 0; count < 12; count++) {
						itoa(mbb_ext_2.cell_mVolts[count], str, 10);
						DEBUG_Println(str);
					}
				default:
					DEBUG_Print("Unknown Command\r\n");
			}
		}
		// Retrive any available Brusa Messages for shits
		int8_t tmp = MCP2515_GetFullReceiveBuffer();
		// itoa(tmp, str, 10);
		// DEBUG_Print(">");
		// DEBUG_Println(str);
		if (tmp == 2) {
			MCP2515_ReadBuffer(&mcp_msg_obj, 0);
			decodeBrusa(&mcp_msg_obj);

			MCP2515_ReadBuffer(&mcp_msg_obj, 1);
			decodeBrusa(&mcp_msg_obj);
		} else if (tmp == 0) { // Receiv Buffer 0 Full
			MCP2515_ReadBuffer(&mcp_msg_obj, tmp);
			decodeBrusa(&mcp_msg_obj);
		} else if (tmp == 1) { //Receive buffer 1 full
			MCP2515_ReadBuffer(&mcp_msg_obj, tmp);
			decodeBrusa(&mcp_msg_obj);
		} 

		if (brusa_message_send) {
			counttt++;
			brusa_message_send = false;
			Brusa_MakeCTL(&brusa_control, &mcp_msg_obj);
			MCP2515_LoadBuffer(0, &mcp_msg_obj);
			MCP2515_SendBuffer(0);
		}

		if (true) {
			uint8_t flag = 0;
			MCP2515_Read(EFLG, &flag, 1);
			if (flag & 0x80) {
				// Buffer 1 Overflow
				MCP2515_BitModify(EFLG, 0x80, 0x00);
				DEBUG_Println("1");
			} else if (flag & 0x40) {
				// Buffer 0 Overflow
				MCP2515_BitModify(EFLG, 0x40, 0x00);
				DEBUG_Println("0");
			}
		}

		if (!RingBuffer_IsEmpty(&rx_buffer)) {
			CCAN_MSG_OBJ_T temp_msg;
			RingBuffer_Pop(&rx_buffer, &temp_msg);
			uint8_t mod_id = temp_msg.mode_id & 0xFF;
			if ((temp_msg.mode_id & MBB_STD_MASK) == MBB_STD) {

				MBB_DecodeStd(&mbb_std, &temp_msg);

				if (newMessage) {
					pack_state.pack_min_mVolts = mbb_std.mod_min_mVolts;
					pack_state.pack_max_mVolts = mbb_std.mod_max_mVolts;
					pack_state.pack_node_min = mod_id;
					pack_state.pack_node_max = mod_id;
					newMessage = false;
				}
				if (mbb_std.mod_min_mVolts < pack_state.pack_min_mVolts) {
					pack_state.pack_min_mVolts = mbb_std.mod_min_mVolts;
					pack_state.pack_node_min = mod_id;
				}

				if (mbb_std.mod_max_mVolts > pack_state.pack_max_mVolts) {
					pack_state.pack_max_mVolts = mbb_std.mod_max_mVolts;
					pack_state.pack_node_max = mod_id;
				}

				itoa(mbb_std.balance_c_count, int_str, 16);
				// DEBUG_Print("Active Balance Circuits: 0x");
				// DEBUG_Println(int_str);

				pack_state.messagesReceived++;
				pack_state.pack_avg_mVolts = (mbb_std.mod_avg_mVolts);
			} else if ((temp_msg.mode_id & MBB_EXT_MASK) == MBB_EXT1 ||
						(temp_msg.mode_id & MBB_EXT_MASK) == MBB_EXT2 ||
						(temp_msg.mode_id & MBB_EXT_MASK) == MBB_EXT3) {
				

				if (mbb_ext_1.id == 0xFF || MBB_GetModID(temp_msg.mode_id) == mbb_ext_1.id) {
					MBB_DecodeExt(&mbb_ext_1, &temp_msg);
					// DEBUG_Println("EXT1");
				} else if (mbb_ext_2.id == 0xFF || MBB_GetModID(temp_msg.mode_id) == mbb_ext_2.id) {
					MBB_DecodeExt(&mbb_ext_2, &temp_msg);
					// DEBUG_Println("EXT2");
				} else {
					DEBUG_Println("Got wierd battery id");
				}
			} else if ((temp_msg.mode_id & 0xF00) == 0x600) {
				uint32_t voltage_mVolts = (temp_msg.data[6] << 8) | temp_msg.data[7];
				voltage_mVolts = (voltage_mVolts + 6000);
				itoa(voltage_mVolts, str, 10);
				DEBUG_Print("Pack Voltage: ");
				DEBUG_Println(str);
			} else {
				DEBUG_Println("Unknown On-Chip MSG");
			}
		}
	}
}

#define TIMED_MESSAGE_DELAY 1000
static uint64_t last_debug_message = 0;
static uint32_t message_count = 0;

int main(void)
{

	Init_Core();
	Init_SM();
	Init_Board();
	Init_Globals();
	Init_CAN();
	Init_Timers();

	// ------------------------------------------------
	// Begin

	DEBUG_Print("Started Up\r\n");

	while(1) {

		uint8_t count;
		if ((count = Chip_UART_Read(LPC_USART, Rx_Buf, UART_RX_BUF_SIZE)) != 0) {
			switch (Rx_Buf[0]) {
				case 'a': // Print out Brusa Mains Info
					DEBUG_Print("Actual Mains Voltage: ");
					itoa(brusa_actual_1.mains_mVolts, str, 10);
					DEBUG_Print(str);
					DEBUG_Print("\r\n");

					DEBUG_Print("Mains type: ");
					itoa(brusa_actual_1.mains_cAmps, str, 10);
					DEBUG_Print(str);
					DEBUG_Print("\r\n");

					DEBUG_Print("Temp: ");
					itoa((brusa_temp.power_temp / 10) - 40 , str, 10);
					DEBUG_Print(str);
					DEBUG_Print("\r\n");

					DEBUG_Print("Temp: 0x");
					itoa(brusa_temp.power_temp , str, 16);
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					break;
				case 'b': // Print out Actual Brusa Output
					DEBUG_Print("Actual Out Voltage: ");
					itoa(brusa_actual_1.output_mVolts, str, 10);
					DEBUG_Println(str);

					DEBUG_Print("Actual Out Current: ");
					itoa(brusa_actual_1.output_cAmps, str, 10);
					DEBUG_Println(str);
					break;
				case 'f': // Print out Pack State
					itoa(pack_state.pack_min_mVolts, str, 10);
					DEBUG_Print("Pack Min Voltage: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					itoa(pack_state.pack_max_mVolts, str, 10);
					DEBUG_Print("Pack Max Voltage: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					itoa(pack_state.pack_avg_mVolts, str, 10);
					DEBUG_Print("Pack Avg Voltage: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");
					itoa(pack_state.messagesReceived, str, 10);
					DEBUG_Print("Messages Received: ");
					DEBUG_Print(str);
					DEBUG_Print("\r\n");

					break;
				case 'y': // Print out Module Balance State
					itoa(mbb_ext_1.bal, str, 2);
					DEBUG_Print("Mod 1: 0b");
					DEBUG_Println(str);
					itoa(mbb_ext_2.bal, str, 2);
					DEBUG_Print("Mod 2: 0b");
					DEBUG_Println(str);
					break;
				case 'e': 
					itoa(brusa_error_message,str, 2);
					DEBUG_Println(str);
					break;
				case 'm':
					DEBUG_Print("Charge Mode: ");
					itoa(Charge_GetMode(), str, 10);
					DEBUG_Println(str);
					break;
				case 'z': // Print out Cell Voltages (Slow)
					for (count = 0; count < 12; count++) {
						itoa(mbb_ext_1.cell_mVolts[count], str, 10);
						DEBUG_Println(str);
					}

					for (count = 0; count < 12; count++) {
						itoa(mbb_ext_2.cell_mVolts[count], str, 10);
						DEBUG_Println(str);
					}
				default:
					DEBUG_Print("Unknown Command\r\n");
			}
		}

		//-----------------------------
		// Detect Input Changes (Default to IDLE)

		MODE_INPUT_T inp = INP_IDLE;
		if (!Board_Switch_Read()) {
			inp = INP_CHRG;
		} else {
			inp = INP_IDLE;
		}

		//-----------------------------
		// Update pack_state
		pack_state.contactors_closed = Board_Contactors_Closed();
		pack_state.msTicks = msTicks;
		pack_state.brusa_error_message = brusa_error_message;

		//-----------------------------
		// SSM Step
		ERROR_T result = SSM_Step(&pack_state, inp, &out_state);
		if (result != ERROR_NONE) {
			_error(result, true, false);
		}

		//-----------------------------
		// Carry out out_state
		if (out_state.close_contactors && !Board_Contactors_Closed()) {
			Board_Close_Contactors(true);
		} else if (!out_state.close_contactors && Board_Contactors_Closed()) {
			Board_Close_Contactors(false);
		}

		if (out_state.brusa_output) {
			brusa_control.clear_error = out_state.brusa_clear_latch;
			brusa_control.output_mVolts = out_state.brusa_mVolts;
			brusa_control.output_cAmps = out_state.brusa_cAmps;
			Chip_TIMER_Enable(LPC_TIMER32_0);
		} else {
			brusa_control.output_mVolts = 0;
			brusa_control.output_cAmps = 0;
			Chip_TIMER_Disable(LPC_TIMER32_0);
		}

		//-----------------------------
		// Retrieve available brusa messages
		int8_t tmp = MCP2515_GetFullReceiveBuffer();
		if (tmp == 2) {
			MCP2515_ReadBuffer(&mcp_msg_obj, 0);
			decodeBrusa(&mcp_msg_obj);
			MCP2515_ReadBuffer(&mcp_msg_obj, 1);
			decodeBrusa(&mcp_msg_obj);
		} else if (tmp == 0) { // Receive Buffer 0 Full
			MCP2515_ReadBuffer(&mcp_msg_obj, tmp);
			decodeBrusa(&mcp_msg_obj);
		} else if (tmp == 1) { //Receive buffer 1 full
			MCP2515_ReadBuffer(&mcp_msg_obj, tmp);
			decodeBrusa(&mcp_msg_obj);
		} 

		//-----------------------------
		// Send brusa message if its time
		if (brusa_message_send) {
			brusa_message_send = false;
			Brusa_MakeCTL(&brusa_control, &mcp_msg_obj);
			MCP2515_LoadBuffer(0, &mcp_msg_obj);
			MCP2515_SendBuffer(0);
		}

		//-----------------------------
		// Set A123 Poll frequency
		// [TODO] Make this happen only on change, not every cycle
		if (SSM_GetMode() != mode) {
			mode = SSM_GetMode();
			switch (SSM_GetMode()) {
				case IDLE:
					Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, Hertz2Ticks(BCM_POLL_IDLE_FREQ));
					Chip_TIMER_Reset(LPC_TIMER32_1);
					break;
				case CHARGING:
					Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, Hertz2Ticks(BCM_POLL_CHARGING_FREQ));
					Chip_TIMER_Reset(LPC_TIMER32_1);
					break;
				case DRAINING:
					Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, Hertz2Ticks(BCM_POLL_DRAINING_FREQ));
					Chip_TIMER_Reset(LPC_TIMER32_1);
					break;
			}

		}
		
		//-----------------------------
		// Check for and decode A123 Messages
		if (!RingBuffer_IsEmpty(&rx_buffer)) {
			CCAN_MSG_OBJ_T temp_msg;
			RingBuffer_Pop(&rx_buffer, &temp_msg);
			uint8_t mod_id = temp_msg.mode_id & 0xFF;
			if ((temp_msg.mode_id & MBB_STD_MASK) == MBB_STD) {

				MBB_DecodeStd(&mbb_std, &temp_msg);

				if (newMessage) {
					pack_state.pack_min_mVolts = mbb_std.mod_min_mVolts;
					pack_state.pack_max_mVolts = mbb_std.mod_max_mVolts;
					pack_state.pack_node_min = mod_id;
					pack_state.pack_node_max = mod_id;
					newMessage = false;
				}
				if (mbb_std.mod_min_mVolts < pack_state.pack_min_mVolts) {
					pack_state.pack_min_mVolts = mbb_std.mod_min_mVolts;
					pack_state.pack_node_min = mod_id;
				}

				if (mbb_std.mod_max_mVolts > pack_state.pack_max_mVolts) {
					pack_state.pack_max_mVolts = mbb_std.mod_max_mVolts;
					pack_state.pack_node_max = mod_id;
				}

				pack_state.messagesReceived++;
				pack_state.pack_avg_mVolts = (mbb_std.mod_avg_mVolts);
			} else if ((temp_msg.mode_id & MBB_EXT_MASK) == MBB_EXT1 ||
						(temp_msg.mode_id & MBB_EXT_MASK) == MBB_EXT2 ||
						(temp_msg.mode_id & MBB_EXT_MASK) == MBB_EXT3) {
				

				if (mbb_ext_1.id == 0xFF || MBB_GetModID(temp_msg.mode_id) == mbb_ext_1.id) {
					MBB_DecodeExt(&mbb_ext_1, &temp_msg);
				} else if (mbb_ext_2.id == 0xFF || MBB_GetModID(temp_msg.mode_id) == mbb_ext_2.id) {
					MBB_DecodeExt(&mbb_ext_2, &temp_msg);
				} else {
					DEBUG_Println("Got wierd battery id");
				}
			} else if ((temp_msg.mode_id & 0xF00) == 0x600) {
				uint32_t voltage_mVolts = (temp_msg.data[6] << 8) | temp_msg.data[7];
				voltage_mVolts = (voltage_mVolts + 6000);
				itoa(voltage_mVolts, str, 10);
				DEBUG_Print("Pack Voltage: ");
				DEBUG_Println(str);
			} else {
				DEBUG_Println("Unknown On-Chip MSG");
			}

		}

		//-----------------------------
		// Timed output
		if (msTicks - last_debug_message > TIMED_MESSAGE_DELAY) {
			message_count++;
			last_debug_message = msTicks;
			switch (message_count % 7) {
				case 0:
					if (out_state.balance) {
						itoa(mbb_cmd.balance_target_mVolts, str, 10);
						DEBUG_Print("Balancing to: ");
						DEBUG_Println(str);
					} else {
						DEBUG_Println("Not balancing");
					}
					
					break;
				case 1:
					itoa(brusa_control.output_mVolts, str, 10);
					DEBUG_Print("Brusa out V: ");
					DEBUG_Println(str);
					break;
				case 2:
					itoa(brusa_control.output_cAmps, str, 10);
					DEBUG_Print("Brusa out C: ");
					DEBUG_Println(str);
					break;
				case 3:
					DEBUG_Print("Actual Out Voltage: ");
					itoa(brusa_actual_1.output_mVolts, str, 10);
					DEBUG_Println(str);
					break;
				case 4:
					DEBUG_Print("Actual Out Current: ");
					itoa(brusa_actual_1.output_cAmps, str, 10);
					DEBUG_Println(str);
					break;
				case 5:
					DEBUG_Print("Mode: ");
					DEBUG_Println((SSM_GetMode() == CHARGING) ? "Chrg":"Idle");
					break;
				case 6:
					DEBUG_Print("Brusa Output: ");
					itoa(out_state.brusa_output, str, 2);
					DEBUG_Println(str);

					DEBUG_Print("\r\n");
					break;

			}
		}
	}
	return 0;
}

