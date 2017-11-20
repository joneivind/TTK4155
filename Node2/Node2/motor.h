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
void motorSpeedPos(uint16_t pos);
void motorDirection(uint8_t dir);
int16_t motorEncoderRead();
void motorResetEncoder();
void motor_test();
uint8_t reverse(uint8_t x);
void setKp(uint8_t * newKp);
void MOTOR_reset();
void MOTOR_control(int8_t xValue);
unsigned char MOTOR_calculateSpeed(int8_t xValue);
int8_t MOTOR_PDcontroller(uint8_t xValue);
int16_t MOTOR_PIDcontroller_Pos(uint8_t pos);

#endif /* MOTOR_H_ */