#include <lpc214x.h>
#define PLOCK 0x00000400

void SystemInit(void)
{
PLL0CON = 0x01;
PLL0CFG = 0x24;
PLL0FEED = 0xAA;
PLL0FEED = 0x55;
while( !( PLL0STAT & PLOCK))
{ ; }
PLL0CON = 0x03;
PLL0FEED = 0xAA; // lock the PLL registers after setting the required PLL
PLL0FEED = 0x55;
VPBDIV = 0x01; // PCLK is same as CCLK i.e 60Mhz
}