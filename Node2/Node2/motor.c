/*
 * motor.c
 *
 * Created: 10.11.2017 12:33:53
 *  Author: jonestr
 */ 
#include "motor.h"
//#include "DAC.h"
#include "TWI_Master.h"
#include <util/delay.h>
#include <avr/interrupt.h>
uint8_t maxSpeed = 255;

static int16_t prevError = 0;

void motor_init(){
	sei();
	//DDRH |= (1 << DDH1);	//DIR pin
	//DDRH |= (1 << DDH3);	//SEL pin
	//DDRH |= (1 << DDH4);	//EN pin
	//DDRH |= (1 << DDH5);	//!0E pin
	//DDRH |= (1 << DDH6);	//RST pin
	//
	//
	////Set PINH4 bit in PORTH register to enable motor (PINH4 = EN pin):
	//PORTH |= (1 << PINH4);
	//
	////Reset the motor:
	//MOTOR_reset();
	DDRH |= (1 << PH1)|(1 << PH4);
	DDRH |= (1 << PH5)|(1 << PH6)|(1 << PH3);
	DDRK = 0x00;
	
	PORTH |= (1 << PH4);
}


void MOTOR_reset(){
	//Clear PINH5 bit in PORTH register to set RST signal thus resetting the motor:
	PORTH &= ~(1 << PINH6);
	_delay_ms(25); //Wait for motor to reset.
	//Set PINH5 bit in PORTH reg to clear RST signal.
	PORTH |= (1 << PINH6);
	//printf("Encoder Reset\n");
}

void motorSpeed(int16_t speed){
	
	speed = (speed - 50)*2;
	//set motor direction
	if(speed < 0)
		motorDirection(0);
	else
		motorDirection(1);
	
	//Cap motor speed
	if(abs(speed) > maxSpeed)
		speed = maxSpeed;
		
	speed = MOTOR_PDcontroller(abs(speed));
	//printf("Speed: %d\n", abs(speed));
	dacWrite(0, speed);
}

void motorDirection(uint8_t dir){
		
	if(dir){
		PORTH &= ~(1 << PINH1);
	}
	else{
		PORTH |= (1 << PINH1);
	}
}

uint8_t motorEncoderRead(){
		//
	//uint16_t encoderVal;
		//
	////Set OE low to enable encoder output:
	////_delay_ms(5000);
	//PORTH &= ~(1 << DDH5);
////	_delay_ms(5000);
	////Set SEL low to get high byte:
	//PORTH &= ~(1 << DDH3);
	////_delay_ms(5000);
	//_delay_us(20);
	//encoderVal = reverse(PINK) << 8; //Read high byte.
	////_delay_ms(5000);
		//
	////printf("high: %d, ", encoderVal);
	////Set SEL high to get low byte:
	//PORTH |= (1 << DDH3);
	////_delay_ms(5000);
	//_delay_us(20);
	//encoderVal |= reverse(PINK); //Read low byte.
////	_delay_ms(5000);
	////printf("low: %d\n", encoderVal);	
	//MOTOR_reset();
	////Set OE high to disable encoder output:
	//PORTH |= (1 << DDH5);
	////_delay_ms(5000);
	//
	//printf("Encoder: %d\n",encoderVal);
		//
	//Return encoder values:
	//return encoderVal;
		uint16_t encoderVal = 0x0000;
		uint8_t MSB;
		uint8_t LSB;
		
		//PORTH |= (1 << PH6);
		
		PORTH &= ~(1 << PH5);
		//_delay_ms(20);
		
		PORTH &= ~(1 << PH3);
		_delay_us(30);
		
		MSB = PINK;
		
		PORTH |= (1 << PH3);
		_delay_us(30);
		
		LSB = PINK;
		printf("Encoder: %d\n", LSB);
		
		PORTH &= ~(1 << PH6);
		
		//PORTH |= (1 << PH5);
		
		encoderVal = (MSB<<8 | LSB);
		_delay_us(140);
		return encoderVal;
}

uint8_t reverse(uint8_t x){
	x = (((x & 0xaa) >> 1) | ((x & 0x55) << 1));
	x = (((x & 0xcc) >> 2) | ((x & 0x33) << 2));
	x = (((x & 0xf0) >> 4) | ((x & 0x0f) << 4));
	return x;
}

void motor_test(){
	
	PORTH |= (1 << DDH6);
	//_delay_ms(2000);
	PORTH &= ~(1 << DDH5);
	//_delay_ms(2000);
}

int8_t MOTOR_PDcontroller(uint8_t speed){

	uint8_t r = speed;// MOTOR_calculateSpeed(xValue);
	uint8_t y = abs(motorEncoderRead());
	
	float kp = 1;
	float kd = 1;
	
	int8_t pError = (r-y);
	int8_t dError = (y-prevError);
	
	int8_t u = kp*pError + kd*dError;
	prevError = y;
	
	
	//printf("r: %d\n", r);
	//printf("y: %d\n", y);
	//printf("u: %d\n\n\n", u);
	return u;
	
}