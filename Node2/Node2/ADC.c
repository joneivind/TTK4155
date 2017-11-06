/*
 * ADC.c
 *
 * Created: 06.11.2017 09:48:48
 *  Author: kjettho
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "ADC.h"

#define SAMPLES 51 // Number of measurements
uint16_t storageArray[SAMPLES] = {0};

void ADC_init(void){
	ADCSRA	|= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADMUX	|= (1<<REFS0);
	ADMUX	&= ~(1<<REFS1);
}

uint16_t medianFilter(uint16_t inputValue)
{	
	uint16_t newVal, temp, temp2;
	uint16_t sorted[SAMPLES] = {0};
	
	newVal = inputValue;
	
	for(int i=0; i<SAMPLES; i++) // >> Shifting values >>
	{
		temp = storageArray[i];
		storageArray[i] = newVal;
		newVal = temp;
	}
	
	for(int i=0; i<SAMPLES;i++) // Create sorting array
		sorted[i] = storageArray[i];
	
	for(int i=0; i<SAMPLES; i++) // Bubble sorting
	{
		for(int j=0; j<SAMPLES; j++)
		{
			if(sorted[i] < sorted[j])
			{
				temp2 = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp2;
			}
		}
	}
	
	return sorted[SAMPLES - (SAMPLES/2)]; // Return median value
}

uint16_t ADC_read(uint8_t channel){
	ADMUX	|= ((1<<channel) & 0x03);
	ADCSRA	|= (1 << ADSC);

	while((ADCSRA & (1<<ADSC))){ ; }

	return medianFilter(ADC);
}