/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 2 Exercise 1
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k){
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	
	while (1) 
    {
		unsigned char count = 0;

		for (unsigned short i = 0; i < 8; i++){	
			tmpA = GetBit(PINA, i);
			tmpB = GetBit(PINB, i);
			if (tmpA == 0x01){
				 count++;
			}
			if (tmpB == 0x01){
				count++;
			}
		}
		
		//output
		PORTC = count;
	
    }
}

