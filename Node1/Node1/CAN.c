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

//Init CAN
void CAN_init()
{
	MCP_init();
	//CAN_setMode(MODE_LOOPBACK);
	CAN_setMode(MODE_NORMAL);
	
	//Make first package with init values
	CAN_message message;
	message.id = 3;
	message.length = 7;
	message.data[0] = 50;
	message.data[1] = 50;
	message.data[2] = 0;
	message.data[3] = 50;
	message.data[4] = 5;
	message.data[5] = 0;
	message.data[6] = 0;
	
	_delay_ms(10);
	
	//Send msg
	CAN_sendMessage(&message);
	
	_delay_ms(10);
}

//Set mode
void CAN_setMode(char mode)
{
	MCP_bitMod(MCP_CANCTRL,MODE_MASK,mode);
}

//Send msg
void CAN_sendMessage(CAN_message * newMessage)
{
	MCP_write(MCP_TXB0SIDL, (newMessage->id << 5));
	MCP_write(MCP_TXB0SIDH, (newMessage->id >> 3));
	MCP_write(MCP_TXB0DLC, newMessage->length);
	
	for(int i=0; i<newMessage->length; i++)
	{
		MCP_write(MCP_TXB0D0 + i, newMessage->data[i]);
	}
	
	MCP_requestToSend(MCP_RTS_TX0);
}

//Recieve msg
void CAN_recieve(CAN_message * newReceivedMessage)
{		
	if(MCP_read(MCP_CANINTF), MCP_RX0IF){
		_delay_ms(10);
		newReceivedMessage->id = (MCP_read(MCP_RXB0SIDH) << 3) | (MCP_read(MCP_RXB0SIDL) >> 5);
		newReceivedMessage->length = MCP_read(MCP_RXB0DLC) & 0x0F;
		for(uint8_t i=0; i<newReceivedMessage->length; i++){
			newReceivedMessage->data[i] = MCP_read(MCP_RXB0D0 + i);
		}
		MCP_bitMod(MCP_CANINTF, MCP_RX0IF, 0);
	}
}

//Print msg
void CAN_printMessage(CAN_message * newReceivedMessage)
{	
	printf("Id: %d\nMsg: ", newReceivedMessage->id);
		
	for(int i=0; i<newReceivedMessage->length; i++)
	{
		 printf("%c", newReceivedMessage->data[i]);
	}
	printf("\n");
}