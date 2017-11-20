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
	uint8_t lives;	
} newgame;

void setLives(uint8_t numLives);
uint16_t get_lives();
void solenoid_init();
void solenoid_trigger(uint8_t mode, uint8_t trigger);

#endif /* GAME_H_ */
