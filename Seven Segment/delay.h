//timer delay
#include<LPC21XX.H>
void delayms(unsigned int ms)
{
	T0PR = 15000;
	T0TCR = 0x01;
	while(T0TC < ms)
	T0TCR = 0x03;
	T0TCR =0x00;
}
