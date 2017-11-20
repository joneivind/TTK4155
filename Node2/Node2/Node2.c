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
#include "motor.h"
#include "motor_controller.h"
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
	motor_init();
	TWI_Master_Initialise();
	
	//Set up CAN messages
	CAN_message message;
	message.id = 2;
	message.length = 1;
	
	CAN_message receivedMessage;
	printf("Initializing node 2...\n");
		
	//Make init constants
	uint8_t kp;
	uint8_t mode = 0;
	uint8_t gameActive = 0;	
	uint8_t motor;
	uint8_t servo;
	uint8_t solenoid;
	
	//Set number of lives
	setLives(10);
	
	while(1)
    {		
		//Check current game mode
        CAN_recieve(&receivedMessage);
		gameActive = receivedMessage.data[6];		
		//CAN_printMessage(&receivedMessage); //Print out CAN for debugging		
		
		//If game mode is active, start game
		if (gameActive)
		{			
			//Set mode and P-value
			setKp(&receivedMessage.data[4]);
			mode = receivedMessage.data[5];
			
			//Get controller data from CAN message
			switch(mode){
				case 0:				
					servo = receivedMessage.data[0];
					solenoid = receivedMessage.data[2];
					motor = receivedMessage.data[3];
				break;
				case 1:
					motor = abs(ADC_read(2))*0.1;
					servo = abs(ADC_read(3))*0.1;
					solenoid = abs(ADC_read(4))*0.1;			
				break;
			}
			
			//Send controller values to servo, solenoid, motor
			PWM_setDutyCycle(servo);
			solenoid_trigger(mode, solenoid);
			motorSpeedPos(motor);
			
			//Check if ball is at bottom
			get_lives(); 
			_delay_ms(10);
			
			//Transmit score back to node 1
			message.data[0] = newgame.lives; 
			printf("Lives: %d\n", message.data[0]);
			CAN_sendMessage(&message);
		}
		else
			setLives(10);				
			_delay_ms(200);
	}
}