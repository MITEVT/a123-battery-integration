/**************************
* 	UART Echo Example     *
* 	Echoes out input      *
***************************/

#include "board.h"
#include "util.h"
#include "mcp2515.h"

#define UART_BAUD 9600
#define SPI_BAUD  115200
#define CCAN_BAUD 500000

#define MCP_BAUD_KHZ  500
#define MCP_INP_CLK_MHZ 11
#define MCP_SJW 1
#define MCP_CS_GPIO 2
#define MCP_CS_PIN 2

// ------------------------------------------------
// Structs

typedef enum {IDLE, CHARGING} MODE_T;
typedef enum {REQ_IDLE, REQ_CHARGING, REQ_NONE} MODE_REQUEST_T;

// ------------------------------------------------
// Global Variables

volatile uint32_t msTicks; // Milliseconds 

static uint8_t Rx_Buf[8]; // UART Receive Software Buffer

static char str[100]; // For use with String Manipulation
static char int_str[100]; // For use within interrupts


static CCAN_MSG_OBJ_T mcp_msg_obj;

// On-Chip CCAN
static CCAN_MSG_OBJ_T can_msg_obj;
static RINGBUFF_T rx_buffer;
CCAN_MSG_OBJ_T _rx_buffer[8];

static MODE_T mode = IDLE;
static MODE_REQUEST_T requested_mode = REQ_NONE;

// ------------------------------------------------
// IRQs

void SysTick_Handler(void) {
	msTicks++;
}

void TIMER32_0_IRQHandler(void) {
	if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 0)) {
		Chip_TIMER_ClearMatch(LPC_TIMER32_0, 0);

		MCP2515_SendBuffer(0);
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
// Main Program

int main(void)
{

	SystemCoreClockUpdate();

	// ------------------------------------------------
	// Systick Config
	if (SysTick_Config (SystemCoreClock / 1000)) {
		//Error
		while(1);
	}

	// ------------------------------------------------
	// Communication Init
	Board_UART_Init(UART_BAUD);
	Board_SPI_Init(SPI_BAUD);
	Board_CCAN_Init(CCAN_BAUD, CAN_rx, CAN_tx, CAN_error);

	// ------------------------------------------------
	// MCP2515 Init
	MCP2515_Init(MCP_CS_GPIO, MCP_CS_PIN);
	uint8_t i = MCP2515_SetBitRate(MCP_BAUD_KHZ, MCP_INP_CLK_MHZ, MCP_SJW);
	itoa(i, str, 10);
	if (i) {
		DEBUG_Print("Baud Error: ");
		DEBUG_Print(str);
		DEBUG_Print("\r\n");
	}

	// ------------------------------------------------
	// On-Chip CCAN Init
	RingBuffer_Init(&rx_buffer, _rx_buffer, sizeof(CCAN_MSG_OBJ_T), 8);
	RingBuffer_Flush(&rx_buffer);

	// ------------------------------------------------
	// LED Init
	Board_LED_Init();
	Board_LED_On();

	// ------------------------------------------------
	// Timer 32_0 Init
	Chip_TIMER_Init(LPC_TIMER32_0);
	Chip_TIMER_Reset(LPC_TIMER32_0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 0);
	Chip_TIMER_SetMatch(LPC_TIMER32_0, 0, SystemCoreClock / 1);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 0);

	// Chip_TIMER_Enable(LPC_TIMER32_0);

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
	NVIC_EnableIRQ(TIMER_32_0_IRQn);

	// ------------------------------------------------
	// Begin
	DEBUG_Print("Started Up\r\n");

	MCP2515_BitModify(RXB0CTRL, RXM_MASK, RXM_OFF);

	mcp_msg_obj.mode_id = 0x600;
	mcp_msg_obj.dlc = 6;
	mcp_msg_obj.data[0] = 0x01;
	mcp_msg_obj.data[1] = 0x02;
	mcp_msg_obj.data[2] = 0x03;
	mcp_msg_obj.data[3] = 0x04;
	mcp_msg_obj.data[4] = 0x05;
	mcp_msg_obj.data[5] = 0x06;

	MCP2515_LoadBuffer(0, &mcp_msg_obj);

	can_msg_obj.msgobj = 1;
	can_msg_obj.mode_id = 0x000;
	can_msg_obj.mask = 0x000;
	LPC_CCAN_API->config_rxmsgobj(&can_msg_obj);

	while(1) {
		uint8_t count;
		if ((count = Chip_UART_Read(LPC_USART, Rx_Buf, 8)) != 0) {
			DEBUG_Write(Rx_Buf, count);
			DEBUG_Print("\r\n");
			switch (Rx_Buf[0]) {
				case 't':
					if (mode == CHARGING) {
						requested_mode = REQ_IDLE;
					} else {
						requested_mode = REQ_CHARGING;
					}
					break;
				case 'i':
					DEBUG_Print("Mode is: ");
					if (mode == CHARGING) {
						DEBUG_Print("CHARGING\r\n");
					} else {
						DEBUG_Print("IDLE\r\n");
					}
					break;
			}
		}

		if (requested_mode != REQ_NONE) {
			if (requested_mode == REQ_IDLE) {
				mode = IDLE;
				Chip_TIMER_Disable(LPC_TIMER32_0);
			} else if (requested_mode == REQ_CHARGING) {
				mode = CHARGING;
				Chip_TIMER_SetMatch(LPC_TIMER32_0, 0, SystemCoreClock / 10); // 100ms
				Chip_TIMER_Enable(LPC_TIMER32_0);
			}

			requested_mode = REQ_NONE;
		}

		if (!RingBuffer_IsEmpty(&rx_buffer)) {
			CCAN_MSG_OBJ_T temp_msg;
			RingBuffer_Pop(&rx_buffer, &temp_msg);
			DEBUG_Print("Received On-Chip CAN. ID: 0x");
			itoa(temp_msg.mode_id, str, 16);
			DEBUG_Print(str);
			DEBUG_Print("\r\n");
		}	

	}

	return 0;
}
