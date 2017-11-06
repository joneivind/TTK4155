/*
 * CAN.h
 *
 * Created: 09.10.2017 15:21:49
 *  Author: jonestr
 */ 


#ifndef CAN_H_
#define CAN_H_
#include <avr/io.h>

typedef struct CAN_MESSAGE{
	uint8_t id;
	uint8_t length;
	uint8_t data[8];
}CAN_message;

void CAN_init();
void CAN_setMode(char mode);
void CAN_sendMessage(CAN_message * message);
void CAN_recieve(CAN_message * receivedMessage);
void CAN_printMessage(CAN_message * msg);
void CAN_PWMPosition(CAN_message * msgPos);

#endif /* CAN_H_ */