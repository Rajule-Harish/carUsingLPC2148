#include <lpc214x.h>

void runDCMotor(int direction,int dutycycle)
{
IO0DIR |= 1U << 28; //set P0.28 as output pin
PINSEL0 |= 2 << 18; //select P0.9 as PWM6 (option 2)
if (direction == 1)
IO0SET = 1 << 28; //set to 1, to choose anti-clockwise direction
else
IO0CLR = 1 << 28; //set to 0, to choose clockwise direction
PWMPCR = (1 << 14); // enable PWM6
PWMMR0 = 1000; // set PULSE rate to value suitable for DC Motor operation
PWMMR6 = (1000U*dutycycle)/100; // set PULSE period
PWMTCR = 0x00000009; // bit D3 = 1 (enable PWM), bit D0=1 (start the timer)
PWMLER = 0X70; // load the new values to PWMMR0 and PWMMR6 registers
}