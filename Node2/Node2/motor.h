/*
 * motor.h
 *
 * Created: 10.11.2017 12:33:44
 *  Author: jonestr
 */ 

#ifndef MOTOR_H_
#define MOTOR_H_

#define MOTOR_DIRECTION 3
#define MOTOR_ENABLE 4
#define MOTOR_SELECT 5
#define MOTOR_RESET 6
#define MOTOR_OE 7

#include <avr/io.h>

void motor_init();
void motorSpeed(int16_t speed);
void motorDirection(uint8_t dir);
uint8_t motorEncoderRead();
void motorResetEncoder();
void motor_test();
uint8_t reverse(uint8_t x);

void MOTOR_reset();
void MOTOR_control(int8_t xValue);
unsigned char MOTOR_calculateSpeed(int8_t xValue);
int8_t MOTOR_PDcontroller(uint8_t xValue);

#endif /* MOTOR_H_ */