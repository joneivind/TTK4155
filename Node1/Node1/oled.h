/*
 * oled.h
 *
 * Created: 25.09.2017 17:27:49
 *  Author: kjettho
 */ 


#ifndef OLED_H_
#define OLED_H_

void oledInit();
void oledClearLine(uint8_t line);
void oledClearScreen();
void oledGotoLine(uint8_t line);
void oledGotoCol(uint8_t column);
void oledPos(uint8_t line, uint8_t column);
void oledWriteData();
void oledHome();
void oledPrintChar(char c);
void oledPrint(char *c);
void oledClearCol();


#endif /* OLED_H_ */