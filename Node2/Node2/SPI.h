/*
 * SPI.h
 *
 * Created: 09.10.2017 10:55:03
 *  Author: jonestr
 */ 


#ifndef SPI_H_
#define SPI_H_
#include <avr/io.h>

#define F_CPU 16000000
#define BAUD 9600
#define BAUDRATE F_CPU/16/BAUD-1

void SPI_init(void);
void SPI_write(char c);
void SPI_slaveEnable(uint8_t select);
char SPI_read();

#endif /* SPI_H_ */