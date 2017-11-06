/*
 * Byggern_group_19.c
 *
 * Created: 04.09.2017 11:07:47
 *  Author: jonestr
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "joystick.h"
#include "oled.h"
#include "menu.h"
#include "SPI.h"
#include "CAN.h"
#include <string.h>

//Input parameters
int maxMinValues[8] = {0,255,0,255,0,255,0,255}; // input ranges - max/min
int joystickValueX = 0;
int joystickValueY = 0;
int sliderValueA = 0;
int sliderValueB = 0;
volatile uint8_t leftButtonPressed = 0;
volatile uint8_t rightButtonPressed = 0;

//Initialize menu system
int currentMenuItem = 0;
int maxItems = 0;
int runningFunction = 0;


int main()
{
	//Initialize commands
	uartInit();
	sramInit();
	OLED_Init();
	menuInit();
	CAN_init();
	
	//Initialize IO ports
	//DDRB = 0x00; //Makes PORTB as Input
	
	//calibrate input
	//setRange(maxMinValues);	
	
	//CAN_message message, receivedMessage;
	//message.id = 3;
	//message.length = 3;
	
	while(1){
		
		//CAN test
		
		//read buttons		
		leftButtonPressed = 0x01 & PINB;
		rightButtonPressed = (0x02 & PINB) >> 1;
		
		//read joystick
		joystickValueX = getControlOutput(1,100,5, &maxMinValues[0], &maxMinValues[1]);		
		joystickValueY = getControlOutput(2,100,5, &maxMinValues[2], &maxMinValues[3]);
		
		/*message.data[0] = joystickValueX;
		message.data[1] = joystickValueY;		
		message.data[2] = leftButtonPressed;
		
		printf("X: %d, Y: %d B: %d\n", message.data[0], message.data[1], message.data[2]);
		
		CAN_sendMessage(&message);
		CAN_recieve(&receivedMessage);*/
		//CAN_printMessage(&receivedMessage);
		//_delay_ms(10);
		
		//Menu
		maxItems = currentMenu.numOfChildren;
		//printf("%s\n", currentMenu.title);
		setMenu(&currentMenu, joystickValueY, leftButtonPressed, &currentMenuItem);		
		
		// Move menu cursor
		if(joystickValueY != 50){						
			if(joystickValueY > 55 && currentMenuItem > 0)
				currentMenuItem--;
			else if(joystickValueY < 45 && currentMenuItem < maxItems-1)
				currentMenuItem++;
			else if(joystickValueY < 45 && currentMenuItem < maxItems && currentMenu.parent != NULL)
				currentMenuItem++;
				
			OLED_ClearCol();
			OLED_Pos(currentMenuItem+2, 1);
			OLED_Print(" >");
			_delay_ms(100);
		}
	}	
	return 0;
}