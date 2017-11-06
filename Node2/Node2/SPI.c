/*
 * SPI.c
 *
 * Created: 09.10.2017 10:54:53
 *  Author: jonestr
 */ 
#include "SPI.h"
#include <avr/io.h>
#include <util/delay.h>

void SPI_init(void){	
	//Set outputs
	DDRB	|=	(1<<DDB0)
			|	(1<<DDB1)
			|	(1<<DDB2)
			|	(1<<DDB7);
			
	//Set CS pin high
	PORTB	|=	(1<<PINB7);
	
	SPCR	|=	(1<<SPE) 
			|	(1<<MSTR) 
			|	(1<<SPR0); // Enable SPI, Master, set clock rate fck/16
}

void SPI_write(char c){
	SPDR = c; // Start transmission
	while(!(SPSR & (1<<SPIF)));	// Wait for transmission complete
}

char SPI_read(){
	SPI_write(0x00);
	uint8_t temp = SPDR;
	return temp;
}

void SPI_slaveEnable(uint8_t enable){	
	if(enable == 1)
		PORTB &= !(1<<PB7);
	else
		PORTB |= (1<<PB7);
}