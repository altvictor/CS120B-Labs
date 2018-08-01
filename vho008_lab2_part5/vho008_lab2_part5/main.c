/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 2 Exercise 5
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

int main(void)
{
	DDRB = 0xFE; PORTB = 0x00;
	DDRD = 0x00; PORTD = 0xFF;
	
	unsigned short weight = 0;
	unsigned char tmpBi = 0x00;			//input for B
	unsigned char tmpBo = 0x00;			//output for B
	
    while (1) 
    {
		//calculate the weight
		tmpBi = PINB & 0x01;
		weight = PIND << 1;
		weight = weight + tmpBi;

		//determine the output
		if(weight >= 70){
			tmpBo = 0x02;
		}
		else if (weight > 5){
			tmpBo = 0x04;
		}
		else{
			tmpBo = 0x00;
		}
		
		//output
		PORTB = tmpBo;
    }
}

