/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 7 Exercise 2
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

typedef struct Task{
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerSet(unsigned long period);
void TimerOn();
void TimerISR();
unsigned long gcd(unsigned long a, unsigned long b);

#define tasksSize 3
task tasks[tasksSize];
unsigned long tasksPeriod = 1;

unsigned char outputThree = 0x00;
unsigned char outputBlink = 0x00;

enum ThreeLED_States;
int ThreeLED_tick(int state);
enum BlinkLED_States;
int BlinkLED_tick(int state);
enum CombineLED_States;
int CombineLED_tick(int state);


void initializeTasks();

int main()
{
	DDRB = 0xFF; PORTB = 0x00;

	//initialize
	initializeTasks();
	TimerSet(tasksPeriod);
	TimerOn();
		
    while (1) {}
	return 0;
}

void initializeTasks(){
	unsigned char index = 0;

	tasks[index].state = -1;
	tasks[index].period = 300;
	tasks[index].elapsedTime = 0;
	tasks[index].TickFct = &ThreeLED_tick;
//	tasksPeriod = tasks[index].period;
	index++;
	
	tasks[index].state = -1;
	tasks[index].period = 1000;
	tasks[index].elapsedTime = 0;
	tasks[index].TickFct = &BlinkLED_tick;
//	tasksPeriod = gcd(tasksPeriod, tasks[index].period);
	index++;
	
	tasks[index].state = -1;
	tasks[index].period = 100;
	tasks[index].elapsedTime = 0;
	tasks[index].TickFct = &CombineLED_tick;
//	tasksPeriod = gcd(tasksPeriod, tasks[index].period);
}


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
		tasks[i].elapsedTime += tasksPeriod;
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

enum ThreeLED_States {Three_start, light0, light1, light2};

int ThreeLED_tick(int state){
	switch (state){ //transitions
		case Three_start:
		state = light0;
		break;
		case light0:
		state = light1;
		break;
		case light1:
		state = light2;
		break;
		case light2:
		state = light0;
		break;
		default:
		state = Three_start;
		break;
	}
	
	switch (state){ //actions
		case Three_start:
		outputThree = 0x00;
		break;
		case light0:
		outputThree = 0x01;
		break;
		case light1:
		outputThree = 0x02;
		break;
		case light2:
		outputThree = 0x04;
		break;
		default:
		break;
	}
	return state;
}

enum BlinkLED_States {Blink_start, lightOn, lightOff};

int BlinkLED_tick(int state){
	switch (state){ //transitions
		case Blink_start:
		state = lightOn;
		break;
		case lightOn:
		state = lightOff;
		break;
		case lightOff:
		state = lightOn;
		break;
		default:
		state = Blink_start;
		break;
	}
	
	switch (state){ //actions
		case Blink_start:
		outputBlink = 0x00;
		break;
		case lightOn:
		outputBlink = 0x01;
		break;
		case lightOff:
		outputBlink = 0x00;
		break;
		default:
		break;
	}
	return state;
}

enum CombineLED_States {Combine_start, display};

int CombineLED_tick(int state){
	unsigned char tmpB = 0x00;
	
	switch (state){ //transitions
		case Combine_start:
		state = display;
		break;
		case display:
		state = display;
		break;
		default:
		state = Combine_start;
		break;
	}
	
	switch (state){ //actions
		case Combine_start:
		tmpB = 0x00;
		break;
		case display:
		tmpB = outputThree;
		tmpB += (outputBlink << 3);
		PORTB = tmpB;
		break;
		default:
		break;
	}
	return state;
}
