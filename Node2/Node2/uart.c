/*
 * uart.c
 *
 * Created: 04.09.2017 14:11:35
 *  Author: TeamNineteen
 */ 
#include <avr/io.h>
#include "uart.h"
#define FOSC 16000000
#define BAUD 9600
#define UBBR FOSC/16/BAUD-1

//Initialize values to enable uart communication
void uartInit()
{
	//Set baud rate
	unsigned int baud = UBBR;
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//Set frame format: 8data, 2stop bit
	UCSR0C = (3<<UCSZ00);	
	//Connect the printf function to self defined uart functions
	fdevopen(uartTransmit, uartReceive);
}

//Transmit data to serial port
void uartTransmit(unsigned char data)
{
	//Wait for empty transmit buffer
	while( !(UCSR0A & (1<<UDRE0)) );
	//Put data into buffer, sends the data
	UDR0 = data;
}

//Read data from serial port
unsigned char uartReceive()
{
	//Wait for data to be received
	while( !(UCSR0A & (1<<RXC0)) );
	//Get and return received data from buffer
	return
	UDR0;
}