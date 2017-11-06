/*
 * joystick.h
 *
 * Created: 21.09.2017 09:44:17
 *  Author: kjettho
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "adc.h"
#include <avr/io.h>
#include <util/delay.h>

int reMap(float input, float minVal, float maxVal, float scale);
int setRange(int *maxMin);
int getControlOutput(int channel, int newScale, int deadzone, int *minVal, int *maxVal);
void getJoystickDirection(int *joyX, int *joyY, int scale, int deadzone);

#endif /* JOYSTICK_H_ */