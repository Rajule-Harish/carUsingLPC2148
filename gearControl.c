#include<lpc214x.h>

void elevatorInit()
{
IO1DIR |= 1 << 24; // to set P1.24 as output
IO1CLR = 1 << 24;
}

void displayGear(char gear)
{
	elevatorInit();
	switch(gear)
	{
		//display res 
		
		case 1:  IO0CLR = 0X000F0000;
						 IO0SET |= 1 << 16;
						 break;
		
		case 2:  IO0CLR = 0X000F0000;
						 IO0SET |= 2 << 16;
						 break;
		
		case 3:  IO0CLR = 0X000F0000;
						 IO0SET |= 3 << 16;
						 break;
		
		case 4:  IO0CLR = 0X000F0000;
						 IO0SET |= 4 << 16;
						 break;

		default: break;
	}
}