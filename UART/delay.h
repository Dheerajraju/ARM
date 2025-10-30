void delay_ms(unsigned int ms)
{
  TOPR = 1500-1;
TOTCR = 0x01;
while(TOTC < ms)
  TOTCR = 0x03;
TOTCR = 0x00;
}

  
