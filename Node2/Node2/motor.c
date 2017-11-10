/*
 * motor.c
 *
 * Created: 10.11.2017 12:33:53
 *  Author: jonestr
 */ 
#include "motor.h"
#include "DAC.h"
#include <util/delay.h>

uint8_t maxSpeed = 255;

void motor_init(){
	
	DDRF	|=	(1<<MOTOR_DIRECTION) 
			|	(1<<MOTOR_ENABLE) 
			|	(1<<MOTOR_SELECT) 
			|	(1<<MOTOR_RESET) 
			|	(1<<MOTOR_OE);
	DDRK = 0;
	PORTF |= (1 << MOTOR_ENABLE);
	
	motorResetEncoder();
}

void motorSpeed(uint16_t speed){
	
	//set motor direction
	if(speed < 0)
		motorDirection(0);
	else
		motorDirection(1);
	
	//Cap motor speed
	if(speed > maxSpeed)
		speed = maxSpeed;
		
	dacWrite(0, speed);
}

void motorDirection(uint8_t dir){
		
	if(dir){
		PORTF &= ~(1 << MOTOR_DIRECTION);
	}
	else{
		PORTF |= (1 << MOTOR_DIRECTION);
	}
}

uint8_t motorEncoderRead(){
	
	uint16_t data;
	
	//Enable output and send high byte on PORT K
	PORTF &= ~((1 << MOTOR_OE) |
	(1 << MOTOR_SELECT));
	_delay_us(20);
	data = (reverse(PINK) << 8);
	
	//Send low byte on PORT K
	PORTF |= (1 << MOTOR_SELECT);
	_delay_us(20);
	data += reverse(PINK);
	
	//Toggle reset and disable output
	PORTF |= (1 << MOTOR_OE);
	
	return data;
}

void motorResetEncoder(){
	PORTF &= ~(1 << MOTOR_RESET);
	_delay_us(10);
	PORTF |= (1 << MOTOR_RESET);
}

void motor_test(){
	motorSpeed(50);
	_delay_ms(500);
	motorSpeed(-50);
	_delay_ms(500);
}