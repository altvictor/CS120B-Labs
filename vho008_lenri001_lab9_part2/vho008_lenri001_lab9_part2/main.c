/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab # Exercise #
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned long tasksPeriod = 100;

enum TickFct_Buttons {start, OnPress, OnWait, UpPress, UpWait, DownPress, DownWait};
int TickFct_Buttons(int state);

typedef struct Task{
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

#define taskSize 1
task tasks[taskSize];

void TimerOn();
void TimerOff();
void TimerISR();
ISR(TIMER1_COMPA_vect);
void TimerSet(unsigned long M);

void set_PWM(double frequency);
void PWM_on();
void PWM_off();

#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)
#define A2 (~PINA & 0x04)

double scale[8] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25};
unsigned char frequency;
unsigned char onSwitch;
// #define C4 261.63
// #define D4 293.66
// #define E4 329.63
// #define F4 349.23
// #define G4 392.00
// #define A4 440.00
// #define B4 493.88
// #define C5 523.25

int main()
{
	//create tasks
	unsigned char i = 0;
	tasks[i].state = start;
	tasks[i].period = 100;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_Buttons;
	i++;
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	PWM_on();
	
	//initialize
	frequency = 0;
	onSwitch = 1;
	set_PWM(scale[frequency]);
	
	while (1) {}
}

int TickFct_Buttons(int state){
	switch (state){ //transitions
		case start:
			if (A0){
				state = OnPress;
			}
			else if (A1 && !A2){
				state = UpPress;
			}
			else if (!A1 && A2){
				state = DownPress;
			}
			else {
				state = start;
			}
			break;
		case OnPress:
			state = OnWait;
			break;
		case OnWait:
			if (A0){
				state = OnWait;
			}
			else if (!A0){
				state = start;
			}
			else {
				state = OnWait;
			}
			break;
		case UpPress:
			state = UpWait;
			break;
		case UpWait:
			if (A0){
				state = OnPress;
			}
			else if (!A1 && A2){
				state = DownPress;
			}
			else if (!A1){
				state = start;
			}
			else {
				state = UpWait;
			}
			break;
		case DownPress:
			state = DownWait;
			break;
		case DownWait:
			if (A0){
				state = OnPress;
			}
			else if (A1 && !A2){
				state = UpPress;
			}
			else if (!A2){
				state = start;
			}
			else {
				state = DownWait;
			}
			break;
		default:
			state = start;
			break;
	}
	
	switch (state){ //actions
		case start:
			break;
		case OnPress:
			if (onSwitch){
				PWM_off();
				onSwitch = 0;
			}
			else {
				PWM_on();
				onSwitch = 1;
			}
			break;
		case UpPress:
			if (frequency < 7){
				frequency++;
			}
			break;
		case DownPress:
			if (frequency > 0){
				frequency--;
			}
			break;
		case OnWait:
		case UpWait:
		case DownWait:
		default:
			break;
	}
	set_PWM(scale[frequency]);
	return state;
}

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
	unsigned char i;
	for (i = 0; i < taskSize; ++i){
		if (tasks[i].elapsedTime >= tasks[i].period){
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += tasksPeriod;
	}
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

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
