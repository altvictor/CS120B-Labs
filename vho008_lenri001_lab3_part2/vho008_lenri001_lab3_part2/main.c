/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 3 Exercise 2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

enum States {start, pressNone, pressA0, pressA1, pressBoth, waitA0, waitA1, waitBoth} state;
	
void tick(){
	switch (state){ //transitions
		case start:
			state = pressNone;
			break;
		case pressNone:
			if (PINA == 0x01) state = pressA0;
			else if (PINA == 0x02) state = pressA1;
			else if (PINA == 0x03) state = pressBoth;
			else state = pressNone;
			break;
		case pressA0:
			state = waitA0;
			break;
		case pressA1:
			state = waitA1;
			break;
		case pressBoth:
			state = waitBoth;
			break;
		case waitA0:
			if (PINA == 0x00) state = pressNone;
			else if (PINA == 0x02) state = pressA1;
			else if (PINA == 0x03) state = pressBoth;
			else state = waitA0;
			break;
		case waitA1:
			if (PINA == 0x00) state = pressNone;
			else if (PINA == 0x01) state = pressA0;
			else if (PINA == 0x03) state = pressBoth;
			else state = waitA1;
			break;
		case waitBoth:
			if (PINA = 0x00) state = pressNone;
			else state = waitBoth;
			break;
		default:
			state = start;
			break;
	}
		
	switch (state){ //actions
		case start:
			PORTC = 0x07;
			break;
		case pressNone:
			break;
		case pressA0:
			if (PORTC < 9) PORTC++;
			break;
		case pressA1:
			if (PORTC > 0) PORTC--;
			break;
		case pressBoth:
			break;
		case waitA0:
			break;
		case waitA1:
			break;
		case waitBoth:
			PORTC = 0x00;
			break;
		default:
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	//initialize
	state = start;
	PORTC = 0x07;
	
    while (1) 
    {
		tick();
	}
}

