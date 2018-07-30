/*
 * vho008_lab1_part3.c
 *
 * Created: 7/30/2018 3:21:03 PM
 * Author : ucrcse
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

