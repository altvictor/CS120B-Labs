/*
 * Victor Ho
 * Lab Section: 21
 * Assignment: Custom Project
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "io.c"
#include "scheduler.h"

char customChar[] = {
	0x0F,
	0x0B,
	0x0F,
	0x0E,
	0x0F,
	0x0E,
	0x0E,
	0x1A,
    NULL
};

//global variables
unsigned char background[] = {'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
                              '_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_', NULL};
unsigned char things[5] = {0, 0, 0, 0, NULL};
unsigned char gameOver;
unsigned short score;
unsigned char P1position;
unsigned char numThings;

#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)
#define A2 (~PINA & 0x04)
#define A3 (~PINA & 0x08)
#define A4 (~PINA & 0x10)

//states
enum tick_Player1 {walk, jump, duck};
int tick_Player1 (int state);

enum tick_Player2 {idle, fireTop, fireMid, fireBot};
int tick_Player2 (int state);

enum tick_Stuff {start, move};
int tick_Stuff (int state);

enum tick_Display {clear, display};
int tick_Display (int state);

//timer
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tasksPeriod = 100;
void TimerOn();
void TimerOff();
void TimerISR();
ISR(TIMER1_COMPA_vect);
void TimerSet(unsigned long M);

#define taskSize 6
task tasks[taskSize];


int main(void)
{
	//declare tasks
	unsigned char i = 0;
// 	tasks[i].state = -1;
// 	tasks[i].period = 200;
// 	tasks[i].elapsedTime = 0;
// 	tasks[i].TickFct = &tick_Player1;
// 	i++;
// 	tasks[i].state = -1;
// 	tasks[i].period = 200;
// 	tasks[i].elapsedTime = 0;
// 	tasks[i].TickFct = &tick_Player2;	
//     i++;
// 	tasks[i].state = -1;
// 	tasks[i].period = 200;
// 	tasks[i].elapsedTime = 0;
// 	tasks[i].TickFct = &tick_Stuff;
//     i++;
    tasks[i].state = -1;
    tasks[i].period = 200;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &tick_Display;
    i++;
    
	//declare ports
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	//initialize
    TimerOn();
    TimerSet(100);
	LCD_init();

    gameOver = 0;
    P1position = 17;
    while (1) {}
}

int tick_Player1 (int state) {
    static unsigned char duration;
    switch (state) {//transitions
        case walk:
            if (A0){
                state = jump;
            }
            else if (A1){
                state = duck;
            }
            else{
                state = walk;
            }
            break;
        case jump:
            if (A0 && duration < 2){
                state = jump;
            }
            else{
                state = walk;
            }
            break;
        case duck:
            state = A1 ? duck : walk;
            break;
        default:
            state = walk;
            break;
    }
    
    switch (state) { //actions
        case walk:
            P1position = 17;
            duration = 0;
            break;
        case jump:
            P1position = 1;
            duration++;
            break;
        case duck:
            P1position = 33;
            break;
        default:
            break;
    }
    return state;
}

int tick_Player2 (int state) {
    static unsigned char charge;
    switch (state) {//transitions
        case idle:
            if (charge < 3){
                state = idle;
            }
            else {
                if (A2){
                state = fireTop;
                }
                else if (A3){
                    state = fireMid;
                }
                else if (A4){
                    state = fireBot;
                }
                else {
                    state = idle;
                }
            }                
            break;
        case fireTop:
            state = idle;
            break;
        case fireMid:
            state = idle;
            break;
        case fireBot:
            state = idle;
            break;
        default:
            state = idle;
            charge = 0;
            break;
    }
    
    switch (state) { //actions
        case idle:
            charge++;
            break;
        case fireTop:
            charge = 0;
            things[numThings] = 16;
            numThings++;
            break;
        case fireMid:
            charge = 0;
            things[numThings] = 32;
            numThings++;
            break;
        case fireBot:
            charge = 0;
            things[numThings] = 48;
            numThings++;
            break;
        default:
            break;
    }
    return state;
}

int tick_Stuff (int state) {
    switch (state) {//transitions
        case start:
            state = move;
            break;
        case move:
            state = move;
            break;
        default:
            state = start;
            break;
    }
    
    switch (state) { //actions
        case start:
            break;
        case move:
            for (unsigned char j = 0; j < numThings; j++){
                things[j]--;
                if (things[j] % 16 == 0){
                    for (unsigned char k = j; k < numThings; k++){
                        things[k] = things[k+1];
                    }
                    numThings--;
                    j--;
                }                    
            }
            break;
        default:
            break;
    }
    return state;
}

int tick_Display (int state) {
    switch (state) { //transitions
        case clear:
            state = display;
            break;
        case display:
            state = display;
            break;
        default:
            state = clear;
            break;
    }
    
    switch (state) { //actions
        case clear:
        	LCD_ClearScreen();
        	LCD_DisplayString(1, background);
            break;
        case display:
            LCD_ClearScreen();
            LCD_DisplayString(1, background);
            LCD_Cursor(P1position);
            LCD_WriteData('&');
            for (unsigned char j = 0; j < numThings; j++){
                LCD_Cursor(j);
                LCD_WriteData('*');
            }
            break;
        default:
            break;
    }
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
	for (unsigned char j = 0; j < taskSize; ++j){
		if (tasks[j].elapsedTime >= tasks[j].period){
			tasks[j].state = tasks[j].TickFct(tasks[j].state);
			tasks[j].elapsedTime = 0;
		}
		tasks[j].elapsedTime += tasksPeriod;
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
