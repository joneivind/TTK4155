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

int8_t error_sum = 0;

static int16_t prevError = 0;

uint16_t encoderVal = 0x0000;

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
	PORTH &= ~(1 << PH6);
	_delay_us(20); //Wait for motor to reset.
	//Set PINH5 bit in PORTH reg to clear RST signal.
	PORTH |= (1 << PH6);
	printf("Encoder Reset\n");
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
		
	//speed = MOTOR_PDcontroller(abs(speed));
	//printf("Speed: %d\n", abs(speed));
	dacWrite(0, abs(speed));
}

void motorSpeedPos(uint16_t * pos){
	
	int16_t speed = MOTOR_PIcontroller_Pos(*pos);
	speed = speed / 20;
	//set motor direction
	if(speed < 0)
		motorDirection(1);
	else
		motorDirection(0);
	
	//Cap motor speed
	if(abs(speed) > maxSpeed)
		speed = maxSpeed;	
	//printf("Speed: %d\n", speed);
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

int16_t motorEncoderRead(){
		uint8_t MSB;
		uint8_t LSB;
		
		//PORTH |= (1 << PH6);
		
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
		////Toggle !RST
		//PORTH &= ~(1 << PH6);
		//_delay_ms(10);
		//PORTH |= (1 << PH6);		
		//Set !OE high
		//PORTH |= (1 << PH5);		
		//_delay_us(140);
		//Process data		
		encoderVal = ((MSB<<8) | LSB);
		//printf("Encoder: %d\t\tLSB: %d\t\tMSB: %d\n", encoderVal, LSB, MSB);
		return encoderVal;
}

void motor_test(){
	int16_t speed = -20;
	uint8_t s = 0;
	while (s < 100)
	{
		motorSpeed(speed);
		s++;
	}
	MOTOR_reset();
	
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

int16_t MOTOR_PIcontroller_Pos(uint8_t pos){

	float kp = 0.9;
	float ki = 1.0;
	float dt = 0.01;
	
	int16_t uMax = 8000;
	uint8_t uMin = 0;
	
	//Convert input to valid range
	int16_t setpoint = uMax * 0.01 * pos;
	
	//Read meas
	int16_t measured_pos = abs(motorEncoderRead());
	
	int16_t error = (setpoint-measured_pos);
		
	error_sum += error;
	
	int16_t pTerm = kp * error;
	int16_t iTerm = ki * error_sum;
	
	int16_t output = pTerm + iTerm;
	
	
	printf("Er: %d\t\tSkal: %d\t\tError: %d\t\tError sum: %d\t\tOutput: %d\n", measured_pos, setpoint,error, error_sum, output);
		
	return output;
}