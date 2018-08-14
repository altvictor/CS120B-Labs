/*
 * Luis Enriquez-Contreras
 * Victor Ho
 * Lab Section: 21
 * Assignment: Lab 3 Exercise 4
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 
#include <avr/io.h>

enum States{ OFF, ON, WAIT} state; // their are states on on0, on1, and off.

const unsigned char output0 = 0x01; // turns on 0th bit
const unsigned char output1 = 0x02; // turns on 1st bit 
unsigned char flag = 0x00;
unsigned char tempA = 0x00; // input


void lightprogram(){
	switch(state) 
	{
		tempA = PINA & 0x01; 

		//goes to default led 0 on for initial state
		case OFF:
		if (tempA != 0x01) {
			state = OFF;}
		else {
			state = ON;}
		break;
		//led 0 is on
		case ON:
		state = WAIT;
		break;
		case WAIT: 
		if (tempA == 0x01) {
			state = WAIT;}
		else {
			state = OFF;}
		break;

		default:  // default is state on zero
			state = OFF;
			break;
	}
	switch(state) // what to do during those state; ie B = 0, B = 1;
	{
		case OFF:
		break;
		
		case ON:
		if (flag == 0x00) {
			flag = 0x01;
			PORTB = 0x02;}
		else {
			flag = 0x00;
			PORTB = 0x01;
		}
		break;
		case WAIT:
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
	flag = 0x00;
	PORTB = 0x01;
	while (1) {
		lightprogram(); // calls function and returns nothing
	}
}