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

uint16_t get_score(){
	uint16_t detect_ball = ADC_read(0);
	
	if(detect_ball <= 10){
		ball_detected = 1;
		newgame.score++;
		printf("Score: %d\n", newgame.score-1);
		while(ADC_read(0) <= 5);
	}
	else
		ball_detected = 0;
}

void solenoid_init(){
	DDRL	|= (1 << DDL0);
	PORTL	|= (1 << PL0);
}

void solenoid_trigger(CAN_message * message){
	
	if(message->data[2]){
		PORTL |= (1 << PL0);
		_delay_ms(50);
	}
	else
		PORTL &= ~(1 << PL0);
}