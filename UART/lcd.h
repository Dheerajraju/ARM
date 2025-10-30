#include <LPC21XX.H>      // Header file for LPC2129 register definitions
#include "delay.h"        // Custom delay header (used for timing between LCD operations)
#define LCD_D 0xFF<<0      // LCD data lines D0–D7 connected to P0.0–P0.7
#define RS    1<<8         // Register Select pin (P0.8)
#define E     1<<9         // Enable pin (P0.9)

void LCD_INIT(void);
void LCD_COMMAND(unsigned char);
void LCD_DATA(unsigned char);
void LCD_STR(unsigned char *);
void LCD_INTEGER(int);

void LCD_INTEGER(int n)
{
	unsigned char arr[5];     // Temporary array to store digits
	signed char i=0;

	if(n==0)                  // If number is 0, just print 0
		LCD_DATA('0');
	else
	{
		if(n<0)               // If negative, display '-' and convert to positive
		{
			LCD_DATA('-');
			n=-n;
		}

		while(n>0)            // Extract digits and store in array
		{
			arr[i++] = n%10;  // Get last digit
			n = n/10;
		}

		for(--i; i>=0; i--)   // Print digits in correct order
			LCD_DATA(arr[i]+48);  // Convert to ASCII ('0' = 48)
	}
}

void LCD_STR(unsigned char *s)
{
	unsigned char count=0;
	while(*s)
	{
		LCD_DATA(*s++);     // Send character to LCD
		count++;
		if(count==16)        // After 16 characters, go to next line
			LCD_COMMAND(0xC0);
	}
}

void LCD_INIT(void)
{
	IODIR0 = LCD_D | RS | E; // Set all LCD pins (P0.0–P0.9) as output

	LCD_COMMAND(0x01);       // Clear LCD
	LCD_COMMAND(0x02);       // Return cursor to home
	LCD_COMMAND(0x0C);       // Display ON, cursor OFF
	LCD_COMMAND(0x38);       // 8-bit, 2-line mode
}

void LCD_COMMAND(unsigned char cmd)
{
    // Send command on P0.0–P0.7
    IOPIN0 = (IOPIN0 & 0xFFFFFF00) | cmd;  // Preserve other bits, set lower 8 bits for LCD data

    IOCLR0 = RS;        // RS=0 → Command mode
    IOSET0 = E;         // Generate Enable pulse
    delay_milliseconds(2);
    IOCLR0 = E;         // End Enable pulse
}

void LCD_DATA(unsigned char d)
{
    IOPIN0 = (IOPIN0 & 0xFFFFFF00) | d;  // Send data to LCD lines

    IOSET0 = RS;        // RS=1 → Data mode
    IOSET0 = E;         // Enable pulse
    delay_milliseconds(2);
    IOCLR0 = E;         // End Enable pulse
}
