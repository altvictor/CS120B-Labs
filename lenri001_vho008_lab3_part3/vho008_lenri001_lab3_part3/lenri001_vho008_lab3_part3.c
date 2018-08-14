/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 3 Exercise 3
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

enum States {start, none, pressX, pressY, pressP, pressIn, 
			waitX, waitY, waitP, waitIn} state;
unsigned char flag = 0x00;

void tick(){
	unsigned char tmpA = PINA & 0x87;
	unsigned char tmpC = 0x00;

	switch (state){ //transitions
		case start:
			state = none;
			break;
		case none:
			if (tmpA == 0x01){
				state = pressX;
			}
			else if (tmpA == 0x02){
				state = pressY;
			}
			else if (tmpA == 0x04){
				state = pressP;
			}
			else if (tmpA == 0x80){
				state = pressIn;
			}
			else {
				state = none;
			}
			break;
		case pressX:
			state = waitX;
			break;
		case pressY:
			state = waitY;
			break;
		case pressP:
			state = waitP;
			break;
		case pressIn:
			state = waitIn;
			break;
		case waitX:
			if (tmpA == 0x00){
				state = none;
			}
			else if (tmpA == 0x01){
				state = waitX;
			}
			else if (tmpA == 0x02){
				state = pressY;
			}
			else if (tmpA == 0x04){
				state = pressP;
			}
			else if (tmpA == 0x80){
				state = pressIn;
			}
			else {
				state = none;
			}
			break;
		case waitY:
			if (tmpA == 0x00){
				state = none;
			}
			else if (tmpA == 0x01){
				state = pressX;
			}
			else if (tmpA == 0x02){
				state = waitY;
			}
			else if (tmpA == 0x04){
				state = pressP;
			}
			else if (tmpA == 0x80){
				state = pressIn;
			}
			else {
				state = none;
			}
			break;
		case waitP:
			if (tmpA == 0x00){
				state = none;
			}
			else if (tmpA == 0x01){
				state = pressX;
			}
			else if (tmpA == 0x02){
				state = pressY;
			}
			else if (tmpA == 0x04){
				state = waitP;
			}
			else if (tmpA == 0x80){
				state = pressIn;
			}
			else {
				state = none;
			}
			break;
		case waitIn:
			if (tmpA == 0x00){
				state = none;
			}
			else if (tmpA == 0x01){
				state = pressX;
			}
			else if (tmpA == 0x02){
				state = pressY;
			}
			else if (tmpA == 0x04){
				state = pressP;
			}
			else if (tmpA == 0x80){
				state = waitIn;
			}
			else {
				state = none;
			}
			break;
		default:
			break;
	}
		
	switch (state){ //actions
		case start:
			break;
		case none:
			tmpC = none;
			break;
		case pressX:
			tmpC = pressX;
			if (flag != 0x02){
				flag = 0x00;
			}
			break;
		case pressY:
			tmpC = pressY;
			if (flag == 0x01){
				flag++;
				PORTB = 0x01;
			}
			else if (flag != 0x02){
				flag = 0x00;
			}
			break;
		case pressP:
			tmpC = pressP;
			if (flag != 0x02){
				flag = 0x01;
			}
			break;
		case pressIn:
			tmpC = pressIn;
			if (flag == 0x02){
				PORTB = 0x00;
			}
			break;
		case waitX:
			tmpC = waitX;
			break;
		case waitY:
			tmpC = waitY;
			break;
		case waitP:
			tmpC = waitP;
			break;
		case waitIn:
			tmpC = waitIn;
			break;
		default:
			tmpC = 0xFF;
			break;
	}
	PORTC = tmpC;
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	//initialize
	state = start;
		
    while (1) 
    {
		tick();
	}
}
