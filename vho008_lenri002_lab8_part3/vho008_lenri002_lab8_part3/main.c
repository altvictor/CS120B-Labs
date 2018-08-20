/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 8 Exercise 3
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

void ADC_init();

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	//initialize
	unsigned char tmpB = 0x00;
	unsigned char tmpD = 0x00;
	unsigned short threshold = 0x0033; //MAX = 0x3F (63), MIN = 0x23 (35)
	unsigned char LEDtoggle = 0x00;
	ADC_init();

    while (1) 
    {
		unsigned short x = ADC;

		tmpB = (char)x;
		tmpD = (char)(x >> 8);
		
		LEDtoggle = (x < threshold) ? 0x00: 0x01;
		tmpD += (LEDtoggle << 1);
	
		PORTB = tmpB;
		PORTD = tmpD;
    }
}

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}
