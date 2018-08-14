/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 4 Exercise 1
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
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char fuel = 0;
	unsigned tmpC = 0x00;
	
    while (1) 
    {
		fuel = PINA & 0x0F;
		//fuel = ~fuel;
		
		if (fuel > 12){
			tmpC = 0x3F;
		}
		else if (fuel > 9){
			tmpC = 0x3E;
		}
		else if (fuel > 6){
			tmpC = 0x3C;
		}
		else if (fuel > 4){
			tmpC = 0x38;
		}
		else if (fuel > 2){
			tmpC = 0x30;
		}
		else if (fuel > 0){
			tmpC = 0x20;
		}
		else {
			tmpC = 0x00;
		}
		
		//low fuel
		if (fuel < 5){
			tmpC = tmpC + 0x40;
		}
		
		//output
		PORTC = tmpC;
    }
}

