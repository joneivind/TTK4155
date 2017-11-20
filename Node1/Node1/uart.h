/*
 * uart.h
 *
 * Created: 04.09.2017 14:11:24
 *  Author: jonestr
 */ 


#ifndef UART_H_

#define UART_H_
#include <avr/io.h>
#include <stdio.h>

void uartInit();
void uartTransmit(unsigned char data);
unsigned char uartReceive();

#endif