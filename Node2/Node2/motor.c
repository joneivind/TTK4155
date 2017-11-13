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
	DDRH |= (1 << PH1)|(1 << PH4);
	DDRH |= (1 << PH5)|(1 << PH6)|(1 << PH3);
	
	PORTH |= (1 << PH4);
	
	PORTH |= (1 << PH5);
	PORTH |= (1 << PH6);
	
	MOTOR_reset();
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
		motorDirection(1);
	else
		motorDirection(0);
	
	//Cap motor speed
	if(abs(speed) > maxSpeed)
		speed = maxSpeed;
		
	speed = MOTOR_PDcontroller(abs(speed));
	//printf("Speed: %d\n", abs(speed));
	dacWrite(0, abs(speed));
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
		uint16_t encoderVal = 0x0000;
		uint8_t MSB;
		uint8_t LSB;
		
		PORTH |= (1 << PH6);
		
		//Set !OE low
		PORTH &= ~(1 << PH5);	
		//Set SEL low	
		PORTH &= ~(1 << PH3);
		_delay_us(20);
		//Read MSB
		MSB = PINK;		
		//printf("MSB: %d\n", MSB);
		//Set SEL high
		PORTH |= (1 << PH3);
		_delay_us(20);
		//Read LSB
		LSB = PINK;		
		//printf("LSB: %d\n", LSB);
		////Toggle !RST
		PORTH &= ~(1 << PH6);
		//_delay_ms(10);
		//PORTH |= (1 << PH6);		
		//Set !OE high
		//PORTH |= (1 << PH5);		
		_delay_us(140);
		//Process data		
		encoderVal = (MSB<<8 | LSB);
		printf("Encoder: %d\n", encoderVal);
		return encoderVal;
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
	
	float kp = -1.0;
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