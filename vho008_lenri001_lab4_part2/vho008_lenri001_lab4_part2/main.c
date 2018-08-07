/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 4 Exercise 2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

#define buttonNone (PINA == 0xFF)
#define buttonA0 (PINA == 0xFE)
#define buttonA1 (PINA == 0xFD)
#define buttonBoth (PINA == 0xFC)
unsigned char count = 0;

enum States {start, pressNone, pressA0, pressA1, pressBoth, waitA0, waitA1, waitBoth} state;
	
void tick(){
	switch (state){ //transitions
		case start:
			state = pressNone;
			break;
		case pressNone:
			if (buttonA0) state = pressA0;
			else if (buttonA1) state = pressA1;
			else if (buttonBoth) state = pressBoth;
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
			if (buttonNone) state = pressNone;
			else if (buttonA1) state = pressA1;
			else if (buttonBoth) state = pressBoth;
			else state = waitA0;
			break;
		case waitA1:
			if (buttonNone) state = pressNone;
			else if (buttonA0) state = pressA0;
			else if (buttonBoth) state = pressBoth;
			else state = waitA1;
			break;
		case waitBoth:
			if (buttonNone) state = pressNone;
			else state = waitBoth;
			break;
		default:
			state = start;
			break;
	}
		
	switch (state){ //actions
		case start:
			count = 0x07;
			break;
		case pressNone:
			break;
		case pressA0:
			if (count < 9) count++;
			break;
		case pressA1:
			if (count > 0) count--;
			break;
		case pressBoth:
			break;
		case waitA0:
			break;
		case waitA1:
			break;
		case waitBoth:
			count = 0x00;
			break;
		default:
			break;
	}
	PORTB = count;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	//initialize
	state = start;
	count = 0x07;
	
    while (1) 
    {
		tick();
	}
}

