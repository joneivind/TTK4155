/*
 * DAC.c
 *
 * Created: 10.11.2017 12:41:10
 *  Author: jonestr
 */ 
#include "DAC.h"
#include "TWI_Master.h"
#include "motor.h"

//Set i2c addresses
#define MAX520_TWI_ADDR_BASE 0b01010000
static uint8_t TWIaddress = 0x00;
static uint8_t TWIaddressVision = 0x10;

//Start i2c
void dacInit(uint8_t address)
{
	TWI_Master_Initialise();
	TWIaddress = address;
}

//Write to i2c
void dacWrite(uint8_t channel, uint8_t val)
{	
	uint8_t msg[3] = { MAX520_TWI_ADDR_BASE | ((TWIaddress & 0x07) << 1), channel & 0x07, val };	
	TWI_Start_Transceiver_With_Data(msg, 3);
}

//Read from i2c
void dacRead()
{
	uint8_t msg[6];
	msg[0] = (TWIaddressVision << 1)| (TRUE<<TWI_READ_BIT);
	TWI_Get_Data_From_Transceiver(msg, 6);
}