/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 5 Exercise 1
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

enum States {start, blink0, blink1, blink2} state;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void tick(){
	unsigned char tmpB = 0x00;

	switch (state){ //transitions
		case start:
			state = blink0;
			break;
		case blink0:
			state = blink1;
			break;
		case blink1:
			state = blink2;
			break;
		case blink2:
			state = blink0;
			break;
		default:
			state = blink0;
			break;
	}
	
	switch (state){ //actions		
		case start:
			tmpB = 0x00;
			break;
		case blink0:
			tmpB = 0x01;
			break;
		case blink1:
			tmpB = 0x02;
			break;
		case blink2:
			tmpB = 0x04;
			break;
		default:
			break;
	}
	PORTB = tmpB;
}
int main()
{
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(1000);
	TimerOn();
	
	//initialize
	state = start;
	
    while (1) 
    {		
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

