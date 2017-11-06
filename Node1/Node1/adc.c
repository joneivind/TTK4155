/*
 * adc.c
 *
 * Created: 18.09.2017 14:37:39
 *  Author: kjettho
 */
 #include <util/delay.h>
#include "adc.h"

volatile char *oledCPointer = (char *) 0x1000; // pointer to OLED command
volatile char *oledDataPointer =(char *) 0x1200; // pointer to OLED Data
volatile char *adcPointer = (char *) 0x1400; // pointer to ADC
volatile char *sramPointer = (char *) 0x1800; // pointer to SRAM
/*
//Initialize adress range
void adcInit(void){
	
}
*/

// ADC read from channel
int adc(int channel){	
	char selectedAxis;
	// Swicth-case depending on MUX channel 	
	switch (channel)
	{
		case 1:
		selectedAxis = 0b00000100;
		break;
		
		case 2:
		selectedAxis = 0b00000101;
		break;
		
		case 3:
		selectedAxis = 0b00000110;
		break;
		
		case 4:
		selectedAxis = 0b00000111;
		break;
		
		default:
		selectedAxis = 0b00000100;
		break;
	}
	
	*adcPointer = selectedAxis;
	
	_delay_ms(10);
	
	return adcPointer[0];
}