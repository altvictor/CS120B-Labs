/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 1 Exercise 3
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	unsigned char cntavail = 0x00;
	unsigned char spot0 = 0x00;
	unsigned char spot1 = 0x00;
	unsigned char spot2 = 0x00;
	unsigned char spot3 = 0x00;

	while (1)
	{
		//read input
		spot0 = PINA & 0x01;
		spot1 = PINA & 0x02;
		spot2 = PINA & 0x04;
		spot3 = PINA & 0x08;
	
		//calculate
		cntavail = 0x04;
	
		if (spot0 == 0x01){
			cntavail--;
		}
		if (spot1 == 0x02){
			cntavail--;
		}
		if (spot2 == 0x04){
			cntavail--;
		}
		if (spot3 == 0x08){
			cntavail--;
		}
	
		if (cntavail == 0x00){
			cntavail = 0x80;
		}
		//output
		PORTC = cntavail;
	}
}

