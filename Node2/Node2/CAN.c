/*
 * CAN.c
 *
 * Created: 09.10.2017 15:21:40
 *  Author: jonestr
 */ 

#include <avr/io.h>
#include "CAN.h"
#include "MCP2515.h"
#include <string.h>
#include <util/delay.h>

//Initiate can bus with spi and mpc
void CAN_init()
{	
	SPI_init();
	MCP_reset();
	
	printf("CAN initiating...\n");

	MCP_bitMod(MCP_RXB0CTRL, 0x60, MCP_FILTER_OFF); // No filter
	MCP_bitMod(MCP_RXB0CTRL, 0x04, MCP_ROLLOVER_OFF); // Disable rollover mode on MCP
	MCP_bitMod(MCP_CANINTE, 0xFF, MCP_RX_INT);
	//MCP_bitMod(MCP_CANCTRL, 0xE0,MODE_LOOPBACK); // uncomment to use loop back mode
	CAN_setMode(MODE_NORMAL); // CAN normal mode
	
	uint8_t canStat = MCP_read(MCP_CANSTAT);
	
	//Print CAN mode
	switch (canStat & MODE_MASK){
		case MODE_NORMAL:
		printf("CAN is in Normal Mode: %d.\n", canStat);
		break;
		case MODE_LOOPBACK:
		printf("CAN is in Loopback Mode: %d.\n", canStat);
		break;
		case MODE_CONFIG:
		printf("CAN is in Config Mode: %d.\n", canStat);
		break;
	}
	printf("CAN initiated!\n");
}

//Set CAN mode
void CAN_setMode(char mode)
{
	MCP_bitMod(MCP_CANCTRL,MODE_MASK,mode);
}

//Send CAN message
void CAN_sendMessage(CAN_message * message)
{	
	MCP_write(MCP_TXB0SIDL, (message->id << 5));
	MCP_write(MCP_TXB0SIDH, (message->id >> 3));
	MCP_write(MCP_TXB0DLC, message->length);
	
	for(int i=0; i<message->length; i++){
		MCP_write(MCP_TXB0D0 + i, message->data[i]);
	}
	
	MCP_requestToSend(MCP_RTS_TX0);
}

//Recieve CAN message
void CAN_recieve(CAN_message * receivedMessage)
{
	//Check if CAN has message
	if(MCP_read(MCP_CANSTAT) & 0x0C){
		receivedMessage->id = (MCP_read(MCP_RXB0SIDH) << 3) | (MCP_read(MCP_RXB0SIDL) >> 5);
		receivedMessage->length = MCP_read(MCP_RXB0DLC) & 0x0F;
		for(uint8_t i=0; i<receivedMessage->length; i++){
			receivedMessage->data[i] = MCP_read(MCP_RXB0D0 + i);
		}
		MCP_bitMod(MCP_CANINTF, MCP_RX0IF, 0);
	}
}

//Print CAN message
void CAN_printMessage(CAN_message * msg)
{	
	printf("Node 2:\nId: %d\n", msg->id);
	printf("Length: %d\n",msg->length);	
	printf("Data: ");
	for(int i=0; i<msg->length; i++){
		 printf("%d, ", msg->data[i]);
	}
	printf("\n");
}
