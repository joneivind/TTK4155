/*
 * SPI.c
 *
 * Created: 09.10.2017 10:54:53
 *  Author: jonestr
 */ 

#include "SPI.h"
#include <avr/io.h>
#include <util/delay.h>

//Set up SPI
void SPI_init(void)
{	
	DDR_SPI |= (1<<DD_MOSI) | (1<<DD_SCK) | (1<<SS_PIN); // Set MOSI and SCK output, all others input
	SPCR |=	(1<<SPE) | (1<<MSTR) | (1<<SPR0); // Enable SPI, Master, set clock rate fck/16
}

//Write char to SPI
void SPI_write(char c)
{
	SPDR = c; // Start transmission
	while(!(SPSR & (1<<SPIF)));	// Wait for transmission complete
}

//Read SPI
char SPI_read()
{
	SPI_write(0);
	uint8_t temp = SPDR;
	return temp;
}

//Slave select on/off
void SPI_slaveEnable(uint8_t enable)
{	
	if(enable)
	PORTB	&=	~(1<<SS_PIN);
	
	else if(!enable)
		PORTB	|=	(1<<SS_PIN);
}