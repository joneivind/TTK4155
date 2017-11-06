/*
 * GAME.h
 *
 * Created: 06.11.2017 10:39:40
 *  Author: kjettho
 */ 


#ifndef GAME_H_
#define GAME_H_
#include "CAN.h"
#include <avr/io.h>
#include <stdio.h>

struct game_stats{
	uint8_t score;	
} newgame;

uint16_t get_score();
void solenoid_init();
void solenoid_trigger(CAN_message * message);

#endif /* GAME_H_ */