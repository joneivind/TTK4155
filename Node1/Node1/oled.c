/*
 * OLED_.c
 *
 * Created: 25.09.2017 17:15:25
 *  Author: kjettho
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "oled.h"
#include <avr/pgmspace.h>
#include "fonts.h"

//Setting addresses to OLED
volatile char *OLED_Command = (char *) 0x1000;
volatile char *OLED_Data = (char *) 0x1200;

//Containers for page, column
uint8_t page, col;

//Goto specific line and column
void OLED_Pos(uint8_t line, uint8_t column)
{	
	OLED_GotoLine(line);
	
	if(column < 128/FONTWIDTH){
		//Save page
		col = column*(FONTWIDTH);
		//Set lower column start address
		*OLED_Command = 0x00 + (column*(FONTWIDTH) >> 4);
		//Set higher column start address
		*OLED_Command = 0x10 + (column*(FONTWIDTH) >> 4);
	}
}

void OLED_PrintChar(char c)
{
	//Write a character
	for (int i = 0; i < FONTWIDTH; i++) {
		*OLED_Data = pgm_read_byte(&font8[c-' '][i]);
	}	
	
}

void OLED_Print(char *c)
{
	int i = 0;	
	char tmp;
	//Write the complete string
	while(c[i] != '\0'){
		OLED_PrintChar(c[i]);
		i++;
	}
}

//Goto specific line
void OLED_GotoLine(uint8_t line)
{	
	OLED_Home();	
	if(line < 8){
		//Save page
		page = line;
		//Set lower column start address
		*OLED_Command = 0x00;
		//Set higher column start address
		*OLED_Command = 0x10;
		//Set page address
		*OLED_Command = 0xB0 | line;
	}
}

//Set the cursor to the start of the screen
void OLED_Home()
{
	*OLED_Command = 0x21;
	*OLED_Command = 0x00;
	*OLED_Command = 0x7f;	
	*OLED_Command = 0x22;
	*OLED_Command = 0x00;
	*OLED_Command = 0x07;	
}

//Clear all characters on a specific line
void OLED_ClearLine(uint8_t line)
{
	//Goto specific line
	OLED_GotoLine(line);
	
	//For all pixels on that line, write empty space
	for(uint8_t i=0; i<128; i++){
		*OLED_Data = 0x00;
	}
	
	OLED_Home();
}

//Clear cursor col
void OLED_ClearCol()
{
	for(uint8_t j=0; j<8; j++){
		OLED_Pos(j, 0);
		
		for (int i = 0; i < FONTWIDTH*2; i++){
			*OLED_Data = 0x00;
		}
	}

}

//Removes all things written on the screen
void OLED_ClearScreen()
{
	for(int page = 0; page < 8; page++){
		*OLED_Command = (0xB0 + page);
		
		for(int i = 0; i < 128; ++i){
			*OLED_Data = 0x00;
		}			
	}
}

//Initialization routine for the OLED_
void OLED_Init()
{
	//Display off
	*OLED_Command = 0xAE;	
	//Segment remap
	*OLED_Command = 0xA1;
	//Common pads hardware: alternative
	*OLED_Command = 0xDA;
	*OLED_Command = 0x12;
	//Common output scan direction:com63~com0
	*OLED_Command = 0xC8;
	//Multiplex ration mode:63
	*OLED_Command = 0xA8;
	*OLED_Command = 0x3F;
	//Display divide ratio/osc. freq. mode
	
	*OLED_Command = 0xD5;
	*OLED_Command = 0x80;
	//Contrast control
	*OLED_Command = 0x81;
	*OLED_Command = 0x50;
	//Set pre-charge period
	*OLED_Command = 0xD9;
	*OLED_Command = 0x21;
	//Set Memory Addressing Mode
	*OLED_Command = 0x20;
	*OLED_Command = 0x02;
	//VCOM deselect level mode
	*OLED_Command = 0xDB;
	*OLED_Command = 0x30;
	//Master configuration
	*OLED_Command = 0xAD;
	*OLED_Command = 0x00;
	//Out follows RAM content
	*OLED_Command = 0xA4;
	//Set normal display
	*OLED_Command = 0xA6;
	//Display on
	*OLED_Command = 0xAF;	
	//Set page start address
	*OLED_Command = 0xB0;
	//Set lower column start address
	*OLED_Command = 0x00;
	//Set higher column start address
	*OLED_Command = 0x10;
	
	//Goto start position
	OLED_ClearScreen();
}