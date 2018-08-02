/*
 * Luis Enriquez-Contreras
 * Victor Ho
 * Lab Section: 21
 * Assignment: Lab 3 Exercise 4
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 
#include <avr/io.h>

enum SM{ OFF, ON0, ON1} state; // their are states on on0, on1, and off.

const unsigned char output0 = 0x01; // turns on 0th bit
const unsigned char output1 = 0x02; // turns on 1st bit 
unsigned char tempA = 0x00; // input

void lightprogram(){
	switch(state) 
	{
		//goes to default led 0 on for initial state
		case OFF:
		state = ON0;
		break;
		//led 0 is on
		case ON0:
		tempA = PINA & 0x01; 
		if(!tempA){// stays in on0 
			state = ON0;}
		
		else if(tempA){ // button is pressed goes to state on1
			state = ON1;}
		break;
		
		case ON1: // led 1 is on
		if(!tempA){ // stays in on1 
			state = ON1;}
		else if(tempA){ // button is pressed goes to state on0
			state = ON0;}
		break;

		default: { // default is state on zero
			state = ON0;}
	}
	switch(state) // what to do during those state; ie B = 0, B = 1;
	{
		case OFF:
		break;
		
		case ON0:
		PORTB = output0; // oth led is on
		break;
		
		case ON1:
		PORTB = output1; // 1st led is on
		break;

		default:
		break;
	}
}

int main(void)
{
	PORTA =  0xFF; DDRA = 0x00; // input
	PORTB = 0x00; DDRB = 0xFF; // output
	
	state = OFF; // initial state
	while (1) {
		lightprogram(); // calls function and returns nothing
		}
}