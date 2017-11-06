#include <stdio.h>
#include "joystick.h"


// Read, remap and auto adjust analog input
int getControlOutput(int channel, int newScale, int deadzone, int *minVal, int *maxVal){

	int result = 0; // stores result
	int analogRead = 0; // stores value from adc
	int scale = newScale; // Scaling input to 0-100

	analogRead = adc(channel); // Reading input from ADC
	_delay_ms(10);

	// Auto adjusting range of input
	if(analogRead < *minVal)
		*minVal = analogRead;
	if(analogRead > *maxVal)
		*maxVal = analogRead;

	// Remapping input from ADC to new scale
	result = reMap(analogRead, *minVal, *maxVal, scale);

	// deadzone
	if(result > scale/2 - deadzone && result < scale/2 + deadzone)
		result = scale/2;

	return result;
}

void getJoystickDirection(int *joyX, int *joyY, int scale, int deadzone){

	int joyXPos = *joyX;
	int joyYPos = *joyY;
	float centerPos = scale/2;

	if (joyYPos > centerPos+deadzone && (joyXPos < centerPos+deadzone && joyXPos > centerPos-deadzone)){ printf("UP\t"); }
	else if (joyYPos > centerPos+deadzone && joyXPos < centerPos-deadzone){ printf("UP-LEFT\t"); }
	else if (joyYPos > centerPos+deadzone && joyXPos > centerPos+deadzone){ printf("UP-RIGHT"); }
	else if ((joyYPos < centerPos+deadzone && joyYPos > centerPos-deadzone) && joyXPos < centerPos-deadzone){ printf("LEFT\t"); }
	else if ((joyYPos < centerPos+deadzone && joyYPos > centerPos-deadzone) && joyXPos > centerPos+deadzone){ printf("RIGHT\t"); }
	else if (joyYPos < centerPos-deadzone && joyXPos < centerPos+deadzone && joyXPos > centerPos-deadzone){ printf("DOWN\t"); }
	else if (joyYPos < centerPos-deadzone && joyXPos < centerPos-deadzone){ printf("DOWN-LEFT"); }
	else if (joyYPos < centerPos-deadzone && joyXPos > centerPos+deadzone){ printf("DOWN-RIGHT"); }
	else { printf("CENTER\t"); }
}


// calibration tool for setting range
int setRange(int *maxMin){
	
	int menu;

	printf("\n\t** Press [1] to calibrate or [any] to continue **\n");
	scanf("%d", &menu);
	
	switch(menu){
		case 0:
			printf("\tExiting...\n");
		break;	

		case 1:
			printf("\n\tStarting calibration process...\n\n");
			for(int i=0; i<4; i++){
				switch(i){
					case 0: printf("\t<<-- Move joystick x-axis between max/min position -->>\n\n"); break;
					case 1: printf("\t<<-- Move joystick y-axis between max/min position -->>\n\n"); break;
					case 2: printf("\t<<-- Move slider A between max/min position -->>\n\n"); break;
					case 3: printf("\t<<-- Move slider B between max/min position -->>\n\n"); break;
				}
				_delay_ms(5000);

				for(int i2=0; i2<300;i2++){
					int analogRead = adc(i+1);
					if(analogRead < maxMin[i])
						maxMin[i] = analogRead;
					if(analogRead > maxMin[i+1])
						maxMin[i+1] = analogRead;
					if(maxMin[i] == 0 && maxMin[i+1] == 255)
						break;
					_delay_ms(1);
					printf("\tMin value: %d\tMax value: %d\n", maxMin[i], maxMin[i+1]);
				}
				printf("\tDone!\n\n");
				if(maxMin[i] >= maxMin[i+1]){ // min value cant be larger or equal to max value
					printf("\tError, calibration failed!\n");
					exit(1);
				}
				else{
					printf("\tCalibration complete!\n\tMin value is: %d, max value is: %d\n\n", maxMin[i], maxMin[i+1]);
				}
			}
			_delay_ms(5000);
		break;
	}

	return 0;
}

// Remapping input to new scale with range from min to max
int reMap(float input, float minVal, float maxVal, float scale){

	// find range of input
	float range = maxVal - minVal;

	if(range == 0) // cannot divide zero
		return scale/2; // return middle position
	
	else {
		int result = scale / range * input;

		if(result > scale)
			return scale; // cap max output value
		else
			return result; // return remapped value
	}
}