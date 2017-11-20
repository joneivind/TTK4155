/*
 * DAC.h
 *
 * Created: 10.11.2017 12:41:26
 *  Author: jonestr
 */ 


#ifndef DAC_H_
#define DAC_H_

#include <avr/io.h>

void dacInit(uint8_t address);
void dacWrite(uint8_t channel, uint8_t value);
void dacRead();

#endif /* DAC_H_ */