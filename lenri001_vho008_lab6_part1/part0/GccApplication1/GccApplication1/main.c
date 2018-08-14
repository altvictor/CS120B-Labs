/*
 * Melissa Quero-Gopar mquer001@ucr.edu Danielle Ye
 * Lab Section: 027 
 * Assignment: Lab 6 Exercise 1
 * I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.

 */ 

#include <avr/io.h>
#include "io.c"

enum States {Start,Init,Increment,Decrement,Reset} state;
unsigned char counter;

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}
// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


void tick(){
	unsigned char tmpB = ~PINA & 0x03;
	switch(state){
		case Start:
		counter = 0x00;
		state = Init;
		break;
		case Init:
		if (tmpB == 0x01 && counter < 9){
			state = Increment;
		}
		else if(tmpB == 0x03){
			state = Reset;
		}
		else if(tmpB == 0x02 && counter > 0){
			state = Decrement;
		}
		else{
			state = Init;
		}
		break;
		case Increment:
			if(tmpB == 0x01){
				state = Init;
			}
			else{
				state = Init;
			}
			break;
		case Decrement:
			if(tmpB == 0x02){
				state = Init;
			}
			else{
				state = Init;
			}
			break;
		
		case Reset:
			state = Init;
			break;
		default:
			state = Start;
			break;
	}
	switch(state){
		case Start: 
			break;
		case Init:
			LCD_Cursor(1);
			LCD_WriteData(counter +'0');
			break;
		case Increment:
			counter = counter + 1;
			LCD_Cursor(1);
			LCD_WriteData(counter +'0');
			break;
		case Decrement:
			counter = counter - 1;
			LCD_Cursor(1);
			LCD_WriteData(counter +'0');
			break;
		case Reset:
			counter = 0x00;
			LCD_Cursor(1);
			LCD_WriteData(counter +'0');
			break;
	}
}

	int main(void)
	{
		DDRC = 0xFF; PORTC = 0x00; // LCD data lines
		DDRD = 0xFF; PORTD = 0x00; // LCD control lines
		DDRA = 0x00; PORTA = 0xFF;
		// Initializes the LCD display
		LCD_init();
		
		// Starting at position 1 on the LCD screen, writes Hello World
		TimerSet(300);
		TimerOn();
		
		while(1) {
			// User code (i.e. synchSM calls)
			
			tick();
			while (!TimerFlag){}	// Wait 1 sec
			TimerFlag = 0;
			// Note: For the above a better style would use a synchSM with TickSM()
			// This example just illustrates the use of the ISR and fla
		}
	}
