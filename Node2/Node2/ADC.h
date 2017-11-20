/*
 * ADC.h
 *
 * Created: 06.11.2017 09:47:39
 *  Author: kjettho
 */ 

#ifndef ADC_H_
#define ADC_H_

void ADC_init();
uint16_t medianFilter(uint16_t inputValue);
uint16_t ADC_read(uint8_t channel);

#endif 