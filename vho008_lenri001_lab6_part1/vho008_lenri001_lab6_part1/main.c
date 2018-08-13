/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 6 Exercise 1
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

enum States {start, pressNone, pressA0, pressA1, pressBoth} state;
	
unsigned char count = 0;

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
	unsigned char tmpA = ~PINA;

	switch (state){ //transitions
		case start:
			state = pressNone;
			break;
		case pressNone:
		case pressA0:
		case pressA1:
		case pressBoth:
			if (tmpA == 0x01) state = pressA0;
			else if (tmpA == 0x02) state = pressA1;
			else if (tmpA == 0x03) state = pressBoth;
			else state = pressNone;
			break;
		default:
			state = pressNone;
			break;
	}
	
	switch (state){ //actions
		case start:
			break;
		case pressNone:
			break;
		case pressA0:
			if (count < 9){
				count++;
			}
			break;
		case pressA1:
			if (count > 0){
				count--;
			}
			break;
		case pressBoth:
			count = 0;
			break;
		default:
			break;
	}
	
	LCD_Cursor(1);
	LCD_WriteData(count + '0');
}
int main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(1000);
	TimerOn();
	
	//initialize
	LCD_init();
	state = start;
	
    while (1) 
    {		
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

