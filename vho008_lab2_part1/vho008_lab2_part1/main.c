/*
 * vho008_lab2_part1.c
 *
 * Created: 7/31/2018 12:43:29 PM
 * Author : ucrcse
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
	
		//read first character of both ports A and B
		tmpA = PINA & 0x01;
		tmpB = PINB & 0x01;

		for (unsigned char i = 0; i < 8; i++){	
			tmpA = (tmpA & (PINA << i)) != 0;
			if (tmpA) count++;
			
		}
		
		//output
		PORTC = count;
	
    }
}

