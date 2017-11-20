/*
 * menu.h
 *
 * Created: 02.10.2017 10:37:02
 *  Author: kjettho
 */ 


#ifndef MENU_H_
#define MENU_H_

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

//Menu elements
struct MenuItem {
	char * title;
	struct MenuItem * parent;
	struct MenuItem * children[8];
	int (*fp)();
	int numOfChildren;
} mainmenu, newgame, settings, highscore, about, calibration, pid, modeselect, misc, currentMenu;

void menuInit();
void addChild(struct MenuItem * child, struct MenuItem * parentName, int (* function)());
void getChildren(struct MenuItem * nodeName);
void setMenu(struct MenuItem * currentMenu, int joystick, int button, int *menuItem);
struct MenuItem * getParent(struct MenuItem * child);
void printMenuToScreen(struct MenuItem * nodeName);
void printCursor(int currentPos, int offset);

void newGame();
void highScore();
void pidSetting();
void modeSetting();
void aboutGame();
void calibrationDummy();

#endif /* MENU_H_ */
