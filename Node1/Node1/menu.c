/*
 * menu.c
 *
 * Created: 02.10.2017 10:31:41
 *  Author: kjettho
 */ 

#include <stdlib.h>
#include <stdio.h>
#include "oled.h"
#include "menu.h"
#include <util/delay.h>
#include <string.h>
#include "joystick.h"
#include "CAN.h"
#include <math.h>

void menuInit(){
	
	OLED_ClearScreen();
	OLED_Pos(2, 1);
	OLED_Print(" >");
	
	mainmenu.title = "Main menu";
	newgame.title = "New game";
	settings.title = "Settings";
	highscore.title = "Highscore";
	about.title = "About";
	misc.title = "Misc";
	sound.title = "Sound";
	calibration.title = "Calibration";
	
	//Main menu
	addChild(&newgame, &mainmenu, &newGame);
	addChild(&highscore, &mainmenu, &highScore);
	addChild(&settings, &mainmenu, NULL);
	addChild(&misc, &mainmenu, NULL);
	
	//Settings menu
	addChild(&sound, &settings, &soundSetting);
	
	//Other menu
	addChild(&calibration, &misc, &calibrationDummy);
	addChild(&about, &misc, &aboutGame);
	
	/*getChildren(&mainmenu);
	newgame.fp(); // run function from menu item node
	getParent(&newgame); // print parent node
	*/
	printMenuToScreen(&mainmenu);	
	//printMenuToScreen(&settings);
	
	currentMenu = mainmenu;
}

// add child to parent node
void addChild(struct MenuItem * child, struct MenuItem * parentName, int (* function)()){	
	child->parent = parentName; // assign parent to child
	parentName->children[parentName->numOfChildren] = child; // assign child to parent
	parentName->numOfChildren++; // add to number of children
	child->fp = function;
}

// get children from parent node
void getChildren(struct MenuItem * nodeName){	
	printf("\nChildren of %s\n", nodeName->title);
	for(int i=0; i<nodeName->numOfChildren; i++){
		printf("%d: %s\n", i, nodeName->children[i]->title);
	}
	printf("\n");
}
				
void setMenu(struct MenuItem * currentMenu, int joystick, int button, int *menuItem){
	
	int maxItems = currentMenu->numOfChildren-1; // Set number of current menu items
	
	if(button){ // On button pressed
		_delay_ms(200); // delay after keystroke
		if(currentMenu->children[*menuItem]->children[0] != NULL){ // if item has child
			*currentMenu = *currentMenu->children[*menuItem]; // set child menu as current menu
			*menuItem = 0;
			OLED_ClearScreen(); // clear oled screen
			printMenuToScreen(currentMenu); // Show menu on screen
			printCursor(*menuItem, 2);
		}
		else if(currentMenu->children[*menuItem]->fp() != NULL){ // else if child has function			
			//OLED_ClearScreen(); // clear oled screen
			//printf("Running function in %s\n", currentMenu->title);
			//currentMenu->children[menuItem]->fp(); // run function
		}
		else if (menuItem == maxItems && currentMenu->parent != NULL){ // if has reached bottom of menu
			OLED_ClearScreen(); // clear oled screen
			currentMenu = currentMenu->parent; // set parent menu as current menu
			printMenuToScreen(currentMenu->parent); // Show menu on screen
		}
	//_delay_ms(500); // delay after keystroke
	}
}

// get children from parent node
void printMenuToScreen(struct MenuItem * nodeName){	
	//char * newTitle = strcat("-",strcat(nodeName->title,"-"));	
	OLED_Pos(0,4);
	OLED_Print(nodeName->title);
	
	for(int i=0; i<nodeName->numOfChildren; i++){
		OLED_Pos(i+2,4);
		OLED_Print(nodeName->children[i]->title);
	}
	if(nodeName->parent != NULL){
		OLED_Pos(nodeName->numOfChildren+2,4);
		OLED_Print("Return");
	}
}

struct MenuItem * getParent(struct MenuItem * child){
	if(child->parent != NULL){
		printf("\nParent of %s -> %s\n", child->title , child->parent->title);
		return &child->parent;
	}
	else{
		printf("No parents...\n");
		return 0;
	}
}

void printCursor(int currentPos, int offset){
	OLED_ClearCol();
	OLED_Pos(currentPos + offset, 1);
	OLED_Print(" >");
}

void newGame(){
	printf("New game\n");
	OLED_ClearScreen();
	
	OLED_Pos(0,0);
	OLED_Print("Game started!");
	
	int maxMinValues[8] = {0,255,0,255,0,255,0,255}; // input ranges - max/min	
	uint8_t joystickValueX = getControlOutput(1,100,5, &maxMinValues[0], &maxMinValues[1]);
	uint8_t joystickValueY = getControlOutput(2,100,5, &maxMinValues[2], &maxMinValues[3]);
	uint8_t sliderValueA = getControlOutput(3,100,5, &maxMinValues[4], &maxMinValues[5]);
	uint8_t leftButtonPressed = 0x01 & PINB;
	
	uint8_t score = 0;
	
	CAN_message message, receivedMessage;
	
	while(1){
		
		joystickValueX = getControlOutput(1,100,5, &maxMinValues[0], &maxMinValues[1]);
		joystickValueY = getControlOutput(2,100,5, &maxMinValues[2], &maxMinValues[3]);
		sliderValueA = getControlOutput(3,100,5, &maxMinValues[4], &maxMinValues[5]);
		leftButtonPressed = 0x01 & PINB;		
		
		CAN_recieve(&receivedMessage);
		score = receivedMessage.data[0]-1;
		
		char joyX[33], joyY[33], buffer[10], buffer2[10], scorebuffer[10];
		itoa(joystickValueX,joyX,10);
		itoa(joystickValueY,joyY,10);
		itoa(score,scorebuffer,10);
		
		strcpy(buffer, "X: ");
		strcat(buffer, joyX);
		strcat(buffer, " Y: ");
		strcat(buffer, joyY);
		strcpy(buffer2, "Score: ");
		strcat(buffer2, scorebuffer);		
		
		//OLED_Pos(1,3);
		//OLED_Print(buffer);
		
		OLED_Pos(2,3);
		OLED_Print(buffer2);
		
		/*if(leftButtonPressed)
			OLED_Pos(6,(int)(13.0f/100.0f*joystickValueX));
		else
			OLED_Pos(7,(int)(13.0f/100.0f*joystickValueX));
		
		OLED_Print("===");
		*/
		message.id = 3;
		message.length = 4;
		
		message.data[0] = joystickValueX;
		message.data[1] = joystickValueY;
		message.data[2] = leftButtonPressed;
		message.data[3] = sliderValueA;
		
		CAN_sendMessage(&message);
		
		/*if(joystickValueX != 50){
			OLED_ClearScreen();
		}*/
		_delay_ms(10);
	}
}
void highScore(){
	printf("highscore\n");
}
void soundSetting(){
	printf("Sound toggle\n");
	sound.title = "Sound on";
}
void aboutGame(){
	printf("about\n");
}
void calibrationDummy(){
	printf("calibration\n");
}