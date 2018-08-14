/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 1 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	
	while(1)
	{
		//read input
		tmpA = PINA;
		
		//calculate
		if (tmpA == 0x01){
			tmpB = (tmpB & 0xFC) | 0x01;
		}
		else {
			tmpB = 0x00;
		}
		
		//output
		PORTB = tmpB;
	}
	return 0;
}

