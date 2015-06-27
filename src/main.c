/**
 * @file main.c
 * @author Eric Ponce
 * @date 23 June 2015
 * @brief Main Program File
 */

#include "board.h"
#include "config.h"
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

#define MBB_STD_MSG_OBJ 1
#define MBB_EXT_1_MSG_OBJ 2
#define MBB_EXT_2_MSG_OBJ 3
#define MBB_EXT_3_MSG_OBJ 4
#define MBB_EXT_4_MSG_OBJ 5
#define BCM_STD_MSG_OBJ 6
#define BCM_EXT_MSG_OBJ 7

#define BCM_POLL_IDLE_FREQ 1
#define BCM_POLL_CHARGING_FREQ 40
#define BCM_POLL_DRAINING_FREQ 40

#define TIMED_MESSAGE_DELAY 1000

#define BRUSA_MAX_MAINS_CAMPS 1500

#define Hertz2Ticks(freq) SystemCoreClock / freq

#define UpdateBCMTimerFreq(freq) (Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, Hertz2Ticks(freq)))

// ------------------------------------------------
// Global Static Variables

static volatile uint64_t msTicks; // Milliseconds 

// Uart
uint8_t Rx_Buf[UART_RX_BUF_SIZE];
static char str[100]; // For use with printing
// static char int_str[100]; // For use with printing in interrupts

// Off-Chip CAN
static CCAN_MSG_OBJ_T mcp_msg_obj;
static NLG5_CTL_T brusa_control;
static NLG5_STATUS_T brusa_status;
static NLG5_ACT_I_T brusa_actual_1;
static NLG5_ACT_II_T brusa_actual_2;
static NLG5_TEMP_T brusa_temp;
static NLG5_ERR_T brusa_error;
static NLG5_MESSAGES_T brusa_messages;
static volatile bool brusa_message_send = false;

// On-Chip CCAN
static CCAN_MSG_OBJ_T can_msg_obj;
static RINGBUFF_T rx_buffer;
static CCAN_MSG_OBJ_T _rx_buffer[CCAN_BUF_SIZE];
static uint8_t std_msg_send_count;

// A123 CAN Message Structures
static MBB_CMD_T mbb_cmd;

// State staging variables
static volatile bool new_std_msg_sent = true;

// State Variables
static MODE_T mode = IDLE;
static PACK_STATE_T pack_state;
static OUTPUT_STATE_T out_state;

// Timed Print
static uint64_t last_debug_message = 0;
static uint32_t message_count = 0;

// ------------------------------------------------
// Helper functions that don't belong in util because systick, printing, etc

/**
 * @details Relies on SysTick and therefore will not function with interrupts 
 * disabled or in a higher priority interrupt
 * 
 * @param ms number of milliseconds to delay
 */
void _delay_ms(uint32_t ms) {
	uint32_t currTicks = msTicks;
	while(msTicks - currTicks < ms) {

	}
}

/**
 * @details Throws error through multiple means: print, flash, and/or hang
 * 
 * @param errorNo The error number
 * @param flashLED set to true to have processor flash the main LED
 * @param hang set to true to hang the processor
 */
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

// ------------------------------------------------
// IRQs

/**
 * @details Increments millisecond counter
 */
void SysTick_Handler(void) {
	msTicks++;
}

/**
 * @details Timer interrupt to maintain Brusa communication timing
 */
void TIMER32_0_IRQHandler(void) {
	if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 0)) {
		Chip_TIMER_ClearMatch(LPC_TIMER32_0, 0);

		brusa_message_send = true;
	}
}

/**
 * @details Timer interrupt to request A123 MBB information
 */
void TIMER32_1_IRQHandler(void) {
	Chip_TIMER_ClearMatch(LPC_TIMER32_1, 0);
	// Update Battery Status
	if (RingBuffer_IsEmpty(&rx_buffer)) {
		// Send BCM_CMD
		mbb_cmd.request_type = BCM_REQUEST_TYPE_STD;
		mbb_cmd.request_id = 5;
		mbb_cmd.balance = out_state.balance;
		mbb_cmd.balance_target_mVolts = out_state.balance_mVolts;

		can_msg_obj.msgobj = BCM_STD_MSG_OBJ;
		MBB_MakeCMD(&mbb_cmd, &can_msg_obj);
		LPC_CCAN_API->can_transmit(&can_msg_obj);	

		new_std_msg_sent = true;
		if (std_msg_send_count++ == 4) {
			std_msg_send_count = 0;

			mbb_cmd.request_type = BCM_REQUEST_TYPE_EXT;
			can_msg_obj.msgobj = BCM_EXT_MSG_OBJ;
			mbb_cmd.request_id = 6;
			MBB_MakeCMD(&mbb_cmd, &can_msg_obj);
			LPC_CCAN_API->can_transmit(&can_msg_obj);

		}
	}
}

// ------------------------------------------------
// CAN Callbacks

/**
 * @details Moves received on-chip CAN messages into ring buffer
 * 
 * @param msg_obj_num CAN message object that received message
 */
void CAN_rx(uint8_t msg_obj_num) {
	/* Determine which CAN message has been received */
	can_msg_obj.msgobj = msg_obj_num;
	/* Now load up the msg_obj structure with the CAN message */
	LPC_CCAN_API->can_receive(&can_msg_obj);
	RingBuffer_Insert(&rx_buffer, &can_msg_obj);
}


/**
 * @details On-chip CAN transmit callback. Currently does nothing
 * 
 * @param msg_obj_num CAN message object that sent message
 */
void CAN_tx(uint8_t msg_obj_num) {
	msg_obj_num++;
}

/**
 * @details On-chip CAN error callback. Currently does nothing
 * 
 * @param error_info error info
 */
void CAN_error(uint32_t error_info) {
	error_info++;
	DEBUG_Print("On-chip CAN Error\n\r");
	// _error(error_info, true, false);
}

// ------------------------------------------------
// Init Functions

/**
 * @details Initializes SysTick Peripheral
 */
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

/**
 * @details Initializes state machines and sets configuration data
 */
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
	PackManager_Init();

}

/**
 * @details Initializes board peripherals
 */
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

/**
 * @details Initializes statically allocated global variables
 */
void Init_Globals(void) {
	brusa_control.enable = 1;
	brusa_control.clear_error = 0;
	brusa_control.ventilation_request = 1;
	brusa_control.max_mains_cAmps = BRUSA_MAX_MAINS_CAMPS;
	brusa_control.output_mVolts = 0;
	brusa_control.output_cAmps = 0;

	pack_state.pack_min_mVolts = 0xFFFFFFFF;
	pack_state.pack_max_mVolts = 0;
	pack_state.pack_cAmps_in = 0;

	out_state.balance_mVolts = BCM_BALANCE_OFF;
	out_state.balance = false;
	out_state.brusa_mVolts = 0;
	out_state.brusa_cAmps = 0;
	out_state.close_contactors = false;
	out_state.brusa_output = false;
	out_state.brusa_clear_latch = false;

	mode = IDLE;

	std_msg_send_count = 0;

	brusa_messages.stat = &brusa_status;
	brusa_messages.act_i = &brusa_actual_1;
	brusa_messages.act_ii = &brusa_actual_2;
	brusa_messages.temp = &brusa_temp;
	brusa_messages.err = &brusa_error;
}

/**
 * @details Initializes both on-chip and off-chip CAN and sets up filters and masks.
 */
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

	// MBB_STD
	can_msg_obj.msgobj = MBB_STD_MSG_OBJ;
	can_msg_obj.mode_id = 0x200;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);

	// MBB_EXT_1
	can_msg_obj.msgobj = MBB_EXT_1_MSG_OBJ;
	can_msg_obj.mode_id = 0x300;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);

	// MBB_EXT_2
	can_msg_obj.msgobj = MBB_EXT_2_MSG_OBJ;
	can_msg_obj.mode_id = 0x400;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);

	// MBB_EXT_3
	can_msg_obj.msgobj = MBB_EXT_3_MSG_OBJ;
	can_msg_obj.mode_id = 0x500;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);

	// MBB_EXT_4
	can_msg_obj.msgobj = MBB_EXT_4_MSG_OBJ;
	can_msg_obj.mode_id = 0x600;
	can_msg_obj.mask = 0xF00;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);
}

/**
 * @details Initiizes timers for timer based interrupts
 */
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

/**
 * @details Where all the magic happens
 * @return Shouldn't return
 */
int main(void) {

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

					break;
				case 'y': // Print out Module Balance State
					itoa(PackManager_GetExtModId(0), str, 16);
					DEBUG_Print("Mod 0x");
					DEBUG_Print(str);
					itoa(PackManager_GetExtBal(0), str, 2);
					DEBUG_Print(": 0b");
					DEBUG_Println(str);
					itoa(PackManager_GetExtModId(1), str, 16);
					DEBUG_Print("Mod 0x");
					DEBUG_Print(str);
					itoa(PackManager_GetExtBal(1), str, 2);
					DEBUG_Print(": 0b");
					DEBUG_Println(str);
					break;
				case 'e': 
					itoa(brusa_error,str, 2);
					DEBUG_Println(str);
					break;
				case 'm': // Print out charge mode and brusa error
					DEBUG_Print("Charge Mode: ");
					itoa(Charge_GetMode(), str, 10);
					DEBUG_Println(str);
					DEBUG_Print("Error Messages: ");
					itoa((uint64_t)brusa_error, str, 2);
					DEBUG_Println(str);
					break;
				// case 'z': // Print out Cell Voltages (Slow)
				// 	for (count = 0; count < 12; count++) {
				// 		itoa(mbb_ext[0].cell_mVolts[count], str, 10);
				// 		DEBUG_Println(str);
				// 	}

				// 	for (count = 0; count < 12; count++) {
				// 		itoa(mbb_ext[1].cell_mVolts[count], str, 10);
				// 		DEBUG_Println(str);
				// 	}
				// 	break;
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
		pack_state.brusa_error = brusa_error;
		pack_state.pack_cAmps_in = brusa_actual_1.output_cAmps;

		//-----------------------------
		// SSM Step
		ERROR_T result = SSM_Step(&pack_state, inp, &out_state);
		if (result != ERROR_NONE) {
			_error(result, true, false);
		}

		//-----------------------------
		// Check if SSM has Changed State
		// Currently only changes Poll Frequency
		// [TODO] Consider setting a status LED!!
		if (SSM_GetMode() != mode) {
			mode = SSM_GetMode();
			switch (SSM_GetMode()) {
				case IDLE:
					Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, Hertz2Ticks(BCM_POLL_IDLE_FREQ));
					Chip_TIMER_Reset(LPC_TIMER32_1); // Otherwise shit gets FUCKED
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
		int8_t res = 0;
		if (tmp == 2) {
			MCP2515_ReadBuffer(&mcp_msg_obj, 0);
			res = Brusa_Decode(&brusa_messages, &mcp_msg_obj);
			if (res == -1) {
				DEBUG_Println("Brusa Decode Error");
				res = 0;
			}
			MCP2515_ReadBuffer(&mcp_msg_obj, 1);
			res = Brusa_Decode(&brusa_messages, &mcp_msg_obj);
		} else if (tmp == 0) { // Receive Buffer 0 Full
			MCP2515_ReadBuffer(&mcp_msg_obj, tmp);
			res = Brusa_Decode(&brusa_messages, &mcp_msg_obj);
		} else if (tmp == 1) { //Receive buffer 1 full
			MCP2515_ReadBuffer(&mcp_msg_obj, tmp);
			res = Brusa_Decode(&brusa_messages, &mcp_msg_obj);
		} 

		if (res == -1) {
			DEBUG_Println("Brusa Decode Error");
			res = 0;
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
		// Check for and decode A123 Messages

		if (!RingBuffer_IsEmpty(&rx_buffer)) {

			CCAN_MSG_OBJ_T temp_msg;
			RingBuffer_Pop(&rx_buffer, &temp_msg);
			res = PackManager_Update(&temp_msg);

			if (new_std_msg_sent) {
				PackManager_Commit(&pack_state);
				new_std_msg_sent = false;
			}
			
		}

		if (res == -1) {
			DEBUG_Println("A123 Decode Error");
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

