/*
 * MCP.c
 *
 * Created: 09.10.2017 10:57:45
 *  Author: jonestr
 */

#include <avr/io.h>
#include "MCP2515.h"
#include "SPI.h"

//Init MCP with SPI
void MCP_init()
{
	SPI_init();
	MCP_reset();
}

//Read from MCP
char MCP_read(char adr)
{
	SPI_slaveEnable(1); //enable CS pin	
	SPI_write(MCP_READ); //set read instruction
	SPI_write(adr); //set address
	uint8_t temp = SPI_read(); //read from address	
	SPI_slaveEnable(0); //disable CS pin		
	return temp;
}

//Write to MCP
void MCP_write(char adr, char data)
{
	SPI_slaveEnable(1); //enable CS pin	
	SPI_write(MCP_WRITE); //set write instruction
	SPI_write(adr); //set address
	SPI_write(data); //write data	
	SPI_slaveEnable(0); //disable CS pin
}

//MCP request to send
void MCP_requestToSend(char txBuffer)
{
	SPI_slaveEnable(1); //enable CS pin	
	SPI_write(MCP_RTS_TX0 | txBuffer); //request-to-send	
	SPI_slaveEnable(0); //disable CS pin
}

//MCP read status
char MCP_readStatus()
{
	SPI_slaveEnable(1); //enable CS pin	
	SPI_write(MCP_READ_STATUS); //read status instruction	
	char temp = SPI_read();
	SPI_slaveEnable(0); //disable CS pin
	return temp;
}

//MCP bitmod
void MCP_bitMod(char adr, char mask, char data)
{
	SPI_slaveEnable(1); //enable CS pin	
	SPI_write(MCP_BITMOD); //bit modify instruction
	SPI_write(adr); //set address
	SPI_write(mask); //set bitmask
	SPI_write(data); //write data	
	SPI_slaveEnable(0); //disable CS pin
}

//MCP reset
void MCP_reset()
{
	SPI_slaveEnable(1); //enable CS pin	
	SPI_write(MCP_RESET); //reset instruction	
	SPI_slaveEnable(0); //disable CS pin
}