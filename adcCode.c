#include<lpc214x.h>

unsigned int adc(int no,int ch)
{
// adc(1,4) for temp sensor LM34, digital value will increase as temp increases
// adc(1,3) for LDR - digival value will reduce as the light increases
// adc(1,2) for trimpot - digital value changes as the pot rotation
unsigned int val;
PINSEL0 |= 0x0F300000; /* Select the P0_13 AD1.4 for ADC function */
/* Select the P0_12 AD1.3 for ADC function */
/* Select the P0_10 AD1.2 for ADC function */
switch (no) //select adc
{
case 0: AD0CR=0x00200600|(1<<ch); //select channel
AD0CR|=(1<<24); //start conversion
while((AD0GDR& (1U<<31))==0);
val=AD0GDR;
break;
case 1: AD1CR=0x00200600|(1<<ch); //select channel
AD1CR|=(1<<24); //start conversion
while((AD1GDR&(1U<<31))==0);
val=AD1GDR;
break;
}
val=(val >> 6) & 0x03FF; // bit 6:15 is 10 bit AD value
return val;
}