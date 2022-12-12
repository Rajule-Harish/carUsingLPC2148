#include<lpc214x.h>
#include "adcCode.c"

char getGear(void)
{
// returns joystic position
// Up , Left , Down, Right & Enter
unsigned int adc_result=0;
char gear = 0;	// neutral
PINSEL0|= 0x0C000000; /* Select the P0_13 AD1.4 for ADC function */
adc_result = adc(1,4);

if(adc_result > 0 && adc_result < 100) gear = 1; // Up
else if(adc_result > 100 && adc_result < 150) gear = 4; // Left
else if(adc_result > 200 && adc_result < 250) gear = 3; // Down
else if(adc_result > 300 && adc_result < 350) gear = 2; // Right
else if(adc_result > 500 && adc_result < 550) gear = 5; // Enter for reverse.
return gear;
}
