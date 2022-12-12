#include <lpc214x.h>
#include <stdio.h>
#include "systemInitCode.c"
#include "joystickControl.c"
#include "gearControl.c"
#include "DCCode.c"

#define LED_OFF (IO0SET = 1U << 31)
#define LED_ON (IO0CLR = 1U << 31)

#define CLUTCH (IO0PIN & (1 << 14))
#define BRAKE (IO0PIN & (1 << 15))
#define ACC (IO1PIN & (1 << 18))

int speed = 0;
int gear = 1;

void delay_ms(unsigned int j)
{
unsigned int x,i;
for(i=0;i<j;i++)
{
for(x=0; x<10000; x++);
}
}

unsigned char getAlphaCode(unsigned char alphachar)
{
	switch (alphachar)
	{
		// dp g f e d c b a - common anode: 0 segment on, 1 segment off
		case '0': return 0xc0;
		case '1': return 0xf9;
		case '2': return 0xa4;
		case '3': return 0xb0;
		case '4': return 0x99;
		case '5': return 0x92;
		case '6': return 0x82;
		case '7': return 0xf8;
		case '8': return 0x80;
		case '9': return 0x98;
		//simmilarly add for other digit/characters
		default : break;
	}
	return 0xff;
}

void alphadisp7SEG(char *buf)
{
	unsigned char i,j;
	unsigned char seg7_data,temp=0;
	for(i=0;i<5;i++) // because only 5 seven segment digits are present
	{
		seg7_data = getAlphaCode(*(buf+i));
		// instead of this look up table can be used
		// to shift the segment data(8bits)to the hardware (shift registers) using Data,Clock,Strobe

		for (j=0 ; j<8; j++)
		{
			//get one bit of data for serial sending
			temp = seg7_data & 0x80; // shift data from Most significan bit (D7)
			if(temp == 0x80)
			IOSET0 |= 1 << 19; //IOSET0 | 0x00080000;
			else
			IOCLR0 |= 1 << 19; //IOCLR0 | 0x00080000;
			//send one clock pulse
			IOSET0 |= 1 << 20; //IOSET0 | 0x00100000;
			delay_ms(1);
			IOCLR0 |= 1 << 20; //IOCLR0 | 0x00100000;

			seg7_data = seg7_data << 1; // get next bit into D7 position

		}
	}
	IOSET0 |= 1 << 30; //IOSET0 | 0x40000000;
	delay_ms(1); //nop();
	IOCLR0 |= 1 << 30; //IOCLR0 | 0x40000000;
	return;
}

void drive()
{
	if (!BRAKE)
	{
		if ((speed - 2) >= 0)
		{
			speed  = speed - 2;
			
			if ((speed < 20) && (gear == 2))
			{
				while (1)
				{
					if ((!CLUTCH) && (getGear() == 1))
					{
						gear = 1;
						displayGear(gear);
						break;
					}
					
					if (!ACC)
					{
						speed = speed + 2;
						break;
					}
				}
			}
			else if ((speed < 40) && (gear == 3))
			{
				while (1)
				{
					if ((!CLUTCH) && (getGear() == 2))
					{
						gear = 2;
						displayGear(gear);
						break;
					}
					
					if (!ACC)
					{
						speed = speed + 2;
						break;
					}
				}
			}
			else if ((speed < 60) && (gear == 4))
			{
				while (1)
				{
					if ((!CLUTCH) && (getGear() == 3))
					{
						gear = 3;
						displayGear(gear);
						break;
					}
					
					if (!ACC)
					{
						speed = speed + 2;
						break;
					}
				}
			}
		}
	}
	else if (!ACC)
	{
		if ((speed + 2) <= 100)
		{
			speed  = speed + 2;	
			
			if ((speed > 20) && (gear == 1))
			{
				while (1)
				{
					if ((!CLUTCH) && (getGear() == 2))
					{
						gear = 2;
						displayGear(gear);
						break;
					}
					
					if (!BRAKE)
					{
						speed  = speed - 2;
						break;
					}
				}
			}
			else if ((speed > 40) && (gear == 2))
			{
				while (1)
				{
					if ((!CLUTCH) && (getGear() == 3))
					{
						gear = 3;
						displayGear(gear);
						break;
					}
					
					if (!BRAKE)
					{
						speed  = speed - 2;
						break;
					}
				}
			}
			else if ((speed > 60) && (gear == 3))
			{
				while (1)
				{
					if ((!CLUTCH) && (getGear() == 4))
					{
						gear = 4;
						displayGear(gear);
						break;
					}
					
					if (!BRAKE)
					{
						speed  = speed - 2;
						break;
					}
				}
			}
		}
	}
	runDCMotor(2, speed);
	delay_ms(200);
}

int main()
{
	IODIR0 |= 1U << 31 | 1 << 19 | 1 << 20 | 1 << 30 | 1 << 16 | 1 << 17 | 1 << 18;
	LED_ON;
	char buf[5];
	SystemInit();
	elevatorInit();
	while (1)
	{
		drive();
		sprintf(buf, "%05d", speed);
		alphadisp7SEG(buf);
  }
}