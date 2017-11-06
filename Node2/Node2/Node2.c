/*
 * Node2.c
 *
 * Created: 23.10.2017 10:59:56
 *  Author: kjettho
 */ 


#include "uart.h"
#include "CAN.h"
#include "PWM.h"
#include "ADC.h"
#include "GAME.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

int main(void)
{
	//Initialize commands
	uartInit();
	CAN_init();
	TC_init();
	ADC_init();
	solenoid_init();
	
	CAN_message message;
	message.id = 2;
	message.length = 1;
	
	CAN_message receivedMessage;
	
	printf("Initializing node 2...\n");
	
	while(1)
    {
        CAN_recieve(&receivedMessage);
        //_delay_ms(10);
		//CAN_printMessage(&receivedMessage);
		
		//_delay_ms(10);
		CAN_PWMPosition(&receivedMessage); //TESING
		printf("ADC: %d ", ADC_read(0));
		get_score();
		printf("Data[2] %d\n", receivedMessage.data[2]);
		//if(receivedMessage.data[2] > 0)
		solenoid_trigger(&receivedMessage);
		
		message.data[0] = newgame.score;
		CAN_sendMessage(&message);
		_delay_ms(10);
    }
}
