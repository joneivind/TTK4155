/*
 * GAME.c
 *
 * Created: 06.11.2017 10:39:53
 *  Author: kjettho
 */

#include "GAME.h"
#include "ADC.h"
#include "CAN.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

uint8_t ball_detected = 0;

//Set initial number of lives
void setLives(uint8_t numLives)
{	
	newgame.lives = numLives;
}

//Checks if ball is at the bottom -> if yes, subtract lives
uint16_t get_lives()
{
	uint16_t detect_ball = medianFilter(ADC_read(1))*10;
	printf("Detect ball: %d\n", detect_ball);
	if(detect_ball <= 3){
		newgame.lives--;
		while(medianFilter(ADC_read(1))*10 <= 3); // Must be activated when score sensors are connected
	}
}

//Setup solenoid
void solenoid_init()
{
	DDRL	|= (1 << DDL0);
	PORTL	|= (1 << PL0);
}

//Trigger solenoid depending on current mode
void solenoid_trigger(uint8_t mode, uint8_t trigger)
{	
	if((mode == 1 && trigger > 80) || mode == 0 && trigger > 0){
		PORTL |= (1 << PL0);
		_delay_ms(50);
	}
	else
		PORTL &= ~(1 << PL0);
}