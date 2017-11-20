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

//Init values for kp(*10), game mode
int8_t kp = 6;
int8_t mode = 0;
int menuIt;

//Create menu system
void menuInit()
{
	//Clear screen
	OLED_ClearScreen();
	OLED_Pos(2, 1);
	OLED_Print(" >");
	
	//Set menu and submenu titles
	mainmenu.title = "Main menu";
	newgame.title = "New game";
	settings.title = "Settings";
	highscore.title = "Highscore";
	about.title = "About";
	misc.title = "Misc";
	pid.title = "PID";
	modeselect.title = "Mode";
	calibration.title = "Calibration";
	
	//Main menu
	addChild(&newgame, &mainmenu, &newGame);
	addChild(&settings, &mainmenu, NULL);
	addChild(&misc, &mainmenu, NULL);
	
	//Settings menu
	addChild(&pid, &settings, &pidSetting);
	addChild(&modeselect, &settings, &modeSetting);
	
	//Other menu
	addChild(&calibration, &misc, &calibrationDummy);
	addChild(&about, &misc, &aboutGame);
	
	// **Debug functions**
	//getChildren(&mainmenu); //Get all children of a menu
	//newgame.fp(); // run function from menu item node
	//getParent(&newgame); // print parent node	
	
	//Make menu
	printMenuToScreen(&mainmenu);
	
	//Update the current menu
	currentMenu = mainmenu;
}

//add child to parent node
void addChild(struct MenuItem * child, struct MenuItem * parentName, int (* function)())
{	
	child->parent = parentName; // assign parent to child
	parentName->children[parentName->numOfChildren] = child; // assign child to parent
	parentName->numOfChildren++; // add to number of children
	child->fp = function;
}

//get children from parent node
void getChildren(struct MenuItem * nodeName)
{	
	printf("\nChildren of %s\n", nodeName->title);
	for(int i=0; i<nodeName->numOfChildren; i++){
		printf("%d: %s\n", i, nodeName->children[i]->title);
	}
	printf("\n");
}

//Update current menu or run function of menu item
void setMenu(struct MenuItem * currentMenu, int joystick, int button, int *menuItem)
{
	int maxItems = currentMenu->numOfChildren; // Set number of current menu items
	menuIt = *menuItem;
	
	if(button){ // On button pressed
		_delay_ms(200); // delay after keystroke
		
		if (*menuItem == maxItems && maxItems >0)
		{
			*currentMenu = *currentMenu->parent; // set parent menu as current menu
			OLED_ClearScreen(); // clear oled screen
			printMenuToScreen(currentMenu); // Show menu on screen
			*menuItem = 0;
			printCursor(*menuItem, 2);
		}
		else if(currentMenu->children[*menuItem]->children[0] != NULL) // if item has child
		{
			*currentMenu = *currentMenu->children[*menuItem]; // set child menu as current menu
			*menuItem = 0;
			OLED_ClearScreen(); // clear oled screen
			printMenuToScreen(currentMenu); // Show menu on screen
			printCursor(*menuItem, 2);
		}
		else if(currentMenu->children[*menuItem]->fp() != NULL){ // else if child has function
		}
	}
}

//Print current menu to screen
void printMenuToScreen(struct MenuItem * nodeName)
{	
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

//Print parent of submenu
struct MenuItem * getParent(struct MenuItem * child)
{
	if(child->parent != NULL){
		printf("\nParent of %s -> %s\n", child->title , child->parent->title);
		return &child->parent;
	}
	else{
		printf("No parents...\n");
		return 0;
	}
}

//Print menu cursor
void printCursor(int currentPos, int offset)
{
	OLED_ClearCol();
	OLED_Pos(currentPos + offset, 1);
	OLED_Print(" >");
}

//Start new game
void newGame()
{
	//Clear scrren
	OLED_ClearScreen();	
	OLED_Pos(1,2);
	OLED_Print("Game started!");
	
	//Set up input values
	int maxMinValues[8] = {0,255,0,255,0,255,0,255};
	uint8_t joystickValueX;
	uint8_t joystickValueY;
	uint8_t sliderValueA;
	uint8_t leftButtonPressed;
	uint8_t rightButtonPressed;
	
	//Mode and lives
	uint8_t gameOn;	
	uint8_t score = 9;
	
	//Create new CAN messages
	CAN_message message, receivedMessage;
	
	//Start loop
	while(1)
	{		
		//Read joystick input		
		joystickValueX = getControlOutput(1,100,5, &maxMinValues[0], &maxMinValues[1]);
		joystickValueY = getControlOutput(2,100,5, &maxMinValues[2], &maxMinValues[3]);
		sliderValueA = getControlOutput(3,100,5, &maxMinValues[4], &maxMinValues[5]);
		leftButtonPressed = 0x01 & PINB;	
		rightButtonPressed = 0x02 & PINB;
		
		//Set game mode active
		gameOn = 1;
				
		//Convert input values to string
		char joyX[33], joyY[33], buffer[10], buffer2[10], scorebuffer[10];
		itoa(joystickValueX,joyX,10);
		itoa(joystickValueY,joyY,10);
		itoa(score,scorebuffer,10);
		
		strcpy(buffer, "X: ");
		strcat(buffer, joyX);
		strcat(buffer, " Y: ");
		strcat(buffer, joyY);
		strcpy(buffer2, "Lives: ");
		strcat(buffer2, scorebuffer);		
		
		//Print lives to screen
		OLED_Pos(3,3);
		OLED_Print(buffer2);
		
		//Setup CAN message with controller values, kp, mode and game status
		message.id = 3;
		message.length = 7;
		
		message.data[0] = joystickValueX;
		message.data[1] = joystickValueY;
		message.data[2] = leftButtonPressed;
		message.data[3] = sliderValueA;
		message.data[4] = kp;
		message.data[5] = mode;
		message.data[6] = gameOn;
		
		//Send msg
		CAN_sendMessage(&message);
		
		_delay_ms(10);
		
		//Receive msg from node 2
		CAN_recieve(&receivedMessage);
		score = receivedMessage.data[0]-1;
		
		//End routine - right button node 1 or no lives left
		if(rightButtonPressed){			
			gameOn = 0;
			message.data[6] = gameOn;
			CAN_sendMessage(&message);
			
			break;
		}
		else if (score <= 0){
			score = 0;
			OLED_ClearScreen();
			OLED_Pos(0,0);
			OLED_Print("Game over!");
			OLED_Pos(2,0);
			OLED_Print("Press any button");
			OLED_Pos(3,0);
			OLED_Print("to continue");			
			
			gameOn = 0;
			message.data[6] = gameOn;
			CAN_sendMessage(&message);
			
			while (1)
			{					
				leftButtonPressed = 0x01 & PINB;
				rightButtonPressed = 0x02 & PINB;
				if(leftButtonPressed || rightButtonPressed)
					break;
			}
			break;
			
		}
	}
	
	
	_delay_ms(200);
	
	OLED_ClearScreen(); // clear oled screen
	printMenuToScreen(&currentMenu); // Show menu on screen
	printCursor(menuIt, 2);
	
}

void highScore()
{
	//printf("highscore\n");
}

//PID menu function
void pidSetting()
{
	int maxMinValues[8] = {0,255,0,255,0,255,0,255}; // input ranges - max/min
	uint8_t joystickValueY;
	uint8_t leftButtonPressed;
	
	OLED_ClearScreen();
	
	OLED_Pos(0,0);
	OLED_Print("Adjust PID");	
	
	char kpVal[10], newbuffer[10];

	while(1){
		//OLED_ClearScreen();
		OLED_Pos(2,2);
		
		joystickValueY = getControlOutput(2,100,5, &maxMinValues[2], &maxMinValues[3]);		
		leftButtonPressed = 0x01 & PINB;
		
		if(joystickValueY > 50){
			kp ++;
			_delay_ms(100);
			OLED_ClearScreen();
		}
		else if(joystickValueY < 50){
			kp --;
			_delay_ms(100);
			OLED_ClearScreen();
		}
		if(kp < 0)
			kp = 0;
			
		itoa(kp,kpVal,10);
		
		strcpy(newbuffer, "Kp: 0.");
		strcat(newbuffer, kpVal);
		
		OLED_Pos(0,0);
		OLED_Print("Adjust PID");
		OLED_Pos(2,2);
		OLED_Print(newbuffer);
		
		if(leftButtonPressed)
			break;
	}
	
	_delay_ms(200);
	//currentMenu = currentMenu->parent;
	OLED_ClearScreen(); // clear oled screen
	printMenuToScreen(&currentMenu); // Show menu on screen
	printCursor(menuIt, 2);	
}	

//About function
void aboutGame()
{
	OLED_ClearScreen();
	
	OLED_Pos(0,0);
	OLED_Print("    About");
	OLED_Pos(2,0);
	OLED_Print("Ping pong game");
	OLED_Pos(4,0);
	OLED_Print("   made by");
	OLED_Pos(6,0);
	OLED_Print("Kjetil Thorsen");
	OLED_Pos(7,0);
	OLED_Print("Jon E. Stranden");
	
	while(1){
		if(0x01 & PINB)
			break;
	}
	
	_delay_ms(200);
	
	OLED_ClearScreen(); // clear oled screen
	printMenuToScreen(&currentMenu); // Show menu on screen
	printCursor(menuIt, 2);
}

void calibrationDummy()
{
	//printf("calibration\n");
}

//Set game controller mode
void modeSetting()
{
	int maxMinValues[8] = {0,255,0,255,0,255,0,255}; // input ranges - max/min
	uint8_t joystickValueY;
	uint8_t leftButtonPressed;
	
	OLED_ClearScreen();
	
	OLED_Pos(0,4);
	OLED_Print("Select mode");	
	OLED_Pos(2,4);
	OLED_Print("Joystick");
	OLED_Pos(3,4);
	OLED_Print("Kinect");
	
	char modeVal[10], newbuffer[10];

	while(1){
		OLED_Pos(2,2);
		
		joystickValueY = getControlOutput(2,100,5, &maxMinValues[2], &maxMinValues[3]);
		leftButtonPressed = 0x01 & PINB;
		
		if(joystickValueY < 50){
			mode ++;
			_delay_ms(100);
			OLED_ClearCol();
		}
		else if(joystickValueY > 50){
			mode --;
			_delay_ms(100);
			OLED_ClearCol();
		}
		
		if(mode < 0)
			mode = 0;
		if(mode > 1)
			mode = 1;
				
		switch(mode){
			case 0:
				OLED_Pos(2,1);
				OLED_Print(" >");
			break;
			case 1:
				OLED_Pos(3,1);
				OLED_Print(" >");;
			break;
		}
		
		itoa(mode,modeVal,10);
		
		strcpy(newbuffer, "Mode: ");
		strcat(newbuffer, modeVal);
		
		OLED_Pos(0,4);
		OLED_Print("Select mode");
		
		if(leftButtonPressed)
			break;
	}
	
	_delay_ms(200);
	
	OLED_ClearScreen(); // clear oled screen
	printMenuToScreen(&currentMenu); // Show menu on screen
	printCursor(0, 2);
}
