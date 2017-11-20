/*
 * motor.c
 *
 * Created: 10.11.2017 12:33:53
 *  Author: jonestr
 */ 
#include "motor.h"
#include "TWI_Master.h"
#include <util/delay.h>
#include <avr/interrupt.h>
uint8_t maxSpeed = 255;

//PID paramenters
float kp = 0.2;
int16_t error_sum = 0;
int16_t last_error = 0;
static int16_t prevError = 0;

//Initial encoder value
uint16_t encoderVal = 0x0000;

//Initialize routine for motor
void motor_init()
{
	sei();
	DDRH |= (1 << PH1)|(1 << PH4);
	DDRH |= (1 << PH5)|(1 << PH6)|(1 << PH3);
	
	PORTH |= (1 << PH4);
	
	PORTH |= (1 << PH5);
	PORTH |= (1 << PH6);
	
	//Initialize encoder value
	MOTOR_reset();
}

//Reset encoder value
void MOTOR_reset()
{
	//set RST signal
	PORTH &= ~(1 << PH6);
	
	_delay_us(20); //Wait for motor to reset.
	
	//Set PIN6 bit in PORTH to clear RST signal.
	PORTH |= (1 << PH6);
}

//Set speed and direction of motor based on absolute position of slider
void motorSpeedPos(uint16_t pos)
{	
	int16_t speed = MOTOR_PIDcontroller_Pos(pos);
	speed = speed / 20;
	
	//set motor direction
	if(speed < 0)
		motorDirection(1);
	else
		motorDirection(0);
	
	//Cap motor speed
	if(abs(speed) > maxSpeed)
		speed = maxSpeed;	
	
	//Write to dac
	dacWrite(0, abs(speed));
}

//Method to set the direction of motor 
void motorDirection(uint8_t dir)
{		
	if(dir){
		PORTH &= ~(1 << PINH1);
	}
	else{
		PORTH |= (1 << PINH1);
	}
}

//Read motor encoder value and store to global variable
int16_t motorEncoderRead()
{
		uint8_t MSB;
		uint8_t LSB;
				
		//Set !OE low
		PORTH &= ~(1 << PH5);	
		//Set SEL low	
		PORTH &= ~(1 << PH3);
		
		_delay_us(20);
		
		//Read MSB
		MSB = PINK;		
		
		//Set SEL high
		PORTH |= (1 << PH3);
		
		_delay_us(20);
		
		//Read LSB
		LSB = PINK;		
		
		//Process data		
		encoderVal = ((MSB<<8) | LSB);
		
		return encoderVal;
}

//Method for setting Kp in the PID controller from from external node
void setKp(uint8_t * newKp)
{
	kp = (*newKp) * 0.1;
}

//PID controller for motor position
int16_t MOTOR_PIDcontroller_Pos(uint8_t pos)
{
	//Read measurement
	int16_t measured_pos = abs(motorEncoderRead());
	float ki = 0.1;
	float kd = 0.01;
	
	int16_t uMax = 8000;
	uint8_t uMin = 0;
	
	//Convert input to valid range
	int16_t setpoint = uMax * 0.01 * pos;
	
	
	int16_t error = (setpoint-measured_pos);
	
	int16_t d_error = last_error - error * 100;
	
	error_sum += error;
	if((abs(error_sum) > uMax) && error_sum >= 0)
		error_sum = uMax;
	else if((abs(error_sum) > uMax) && error_sum <= 0)
		error_sum = -uMax;	
	
	int16_t pTerm = kp * error;
	int16_t iTerm = ki * error_sum;
	int16_t dTerm = kd * d_error;
	
	int16_t output = pTerm + iTerm + dTerm;
	if(abs(output) > uMax)
		output = uMax;
	else if((abs(output) > uMax) && output <= 0)
		output = -uMax;
		
	last_error = error;
		
	return output;
}