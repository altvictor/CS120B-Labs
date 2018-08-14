//timer.h
#include "taskScheduler.h"

const unsigned char tasksSize;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerSet(unsigned long period){
	_avr_timer_M = period;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerISR(){
	unsigned char i;
	for (i = 0; i < tasksSize; ++i){
		if (tasks[i].elapsedTime >= tasks[i].period){
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += tasks[i].period;
	}
}

void TimerOff(){
	TCCR1B = 0x00;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

unsigned long gcd(unsigned long a, unsigned long b){
	unsigned long remainder = a % b;
	if (remainder == 0){
		return b;
	}
	return gcd(b,remainder);
}