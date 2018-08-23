/*
 * Victor Ho
 * Lab Section: 21
 * Assignment: Custom Project
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 
#include <avr/io.h>
#include "io.c"

unsigned char customChar[] = {
	0x0F,
	0x0B,
	0x0F,
	0x0E,
	0x0F,
	0x0E,
	0x0E,
	0x1A
};

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	LCD_init();
	LCD_ClearScreen();
	LCD_WriteCommand(0x40);
	LCD_WriteData(customChar);
	LCD_WriteCommand(0x80);
	LCD_DisplayString(20, "hello there");
    while (1) {}
}
