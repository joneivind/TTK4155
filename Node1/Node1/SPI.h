/*
 * SPI.h
 *
 * Created: 09.10.2017 10:55:03
 *  Author: jonestr
 */ 


#ifndef SPI_H_
#define SPI_H_
#include <avr/io.h>

#define DDR_SPI DDRB // SPI interface
#define DD_SCK PB7 // SPI clock
#define DD_MISO PB6 // MISO pin
#define DD_MOSI PB5 // MOSI pin
#define SS_PIN PB4 // Slave select pin

void SPI_init(void);
void SPI_write(char c);
void SPI_slaveEnable(uint8_t select);
char SPI_read();

#endif /* SPI_H_ */