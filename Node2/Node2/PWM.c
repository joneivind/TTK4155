/*
 * PWM.c
 *
 * Created: 02.11.2017 17:56:25
 *  Author: kjettho
 */ 

#include <avr/interrupt.h>
#include "CAN.h"

void TC_init(){

	TCCR3A |= (1 << WGM30) | (1 << WGM31);
	TCCR3B |= (1 << WGM32) | (1 << WGM33);

	DDRE |= (1 << DDE3) | (1 << DDE4) | (1 << DDE5);

	OCR3AH = 0xA4;
	OCR3AL = 0x0F;

	TCCR3A |= (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) ;
	TCCR3A &= ~((1 << COM3A0) | (1 << COM3B0) | (1 << COM3C0));	

	TCCR3B |= (1 << CS31);
	TCCR3B &= ~((1 << CS30) | (1 << CS32));

}

void PWM_setDutyCycle(uint8_t xValue)
{
  	uint16_t dutyC = (1800+(xValue*20));

	if(dutyC < 1800){
		dutyC = 1800;
	}
	else if(dutyC > 4197){
		dutyC = 4197;
	}
	int high = (dutyC >> 8);
	int low = (dutyC & 0xff);
	OCR3BL = low;
	OCR3BH = high;
}