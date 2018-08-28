/*
 * Victor Ho
 * Lab Section: 21
 * Assignment: Custom Project
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
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
	0x1A
};

char customCharDuck[] = {
    0x00,
    0x00,
    0x00,
    0x07,
    0x05,
    0x1f,
    0x18,
    0x10
};

char customCactus[] = {
	0x02,
	0x07,
	0x17,
	0x17,
	0x1F,
	0x07,
	0x07,
	0x07
};

char customGhost[] = {
	0x0E,
	0x1F,
	0x15,
	0x1F,
	0x1F,
	0x15,
	0x00,
	0x00,
};

char customGhostTop = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0E,
	0x1F,
	0x1F
};

char customGhostBot = {
	0x1F,
	0x15,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00
};

//global variables
unsigned char background[] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                              '_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_', NULL};
unsigned char things[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char gameOver;
unsigned char score;
unsigned char P1position;
unsigned char numThings;

#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)
#define A2 (~PINA & 0x04)
#define A3 (~PINA & 0x08)
#define A4 (~PINA & 0x10)
#define A5 (~PINA & 0x20)

//states
enum tick_Player1 {walk, jump, duck, p1stop};
int tick_Player1 (int state);

enum tick_Player2 {idle, fireTop, fireMid, fireBot, p2stop};
int tick_Player2 (int state);

enum tick_Stuff {start, move, stop};
int tick_Stuff (int state);

enum tick_Game {init, play, result};
int tick_Game (int state);

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

#define taskSize 5
task tasks[taskSize];

int main(void)
{
	//declare tasks
	unsigned char i = 0;
	tasks[i].state = -1;
	tasks[i].period = 100;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &tick_Player1;
	i++;
	tasks[i].state = -1;
	tasks[i].period = 200;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &tick_Player2;	
    i++;
	tasks[i].state = -1;
	tasks[i].period = 400;//400 for actual
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &tick_Stuff;
    i++;
	tasks[i].state = -1;
	tasks[i].period = 200;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &tick_Game;
	i++;
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
    TimerSet(tasksPeriod);
    TimerOn();
	LCD_init();
	LCD_WriteCommand(0x0C);
    LCD_Custom_Char(1, customChar);
	LCD_Custom_Char(2, customCharDuck);
	LCD_Custom_Char(3, customCactus);
	LCD_Custom_Char(4, customGhost);
	LCD_Custom_Char(5, customGhostTop);
	LCD_Custom_Char(6, customGhostBot);	
	
    gameOver = 0;
    P1position = 18;

    while (1) {}
}

int tick_Player1 (int state) {
    static unsigned char duration;
    switch (state) {//transitions
        case walk:
			if (gameOver){
				state = p1stop;
			}
            else if (A0){
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
			if (gameOver){
				state = p1stop;
			}
            else if (duration < 8){
                state = jump;
            }
            else{
                state = walk;
            }
            break;
        case duck:
			if (gameOver){
				state = p1stop;
			}
			else {
				state = A1 ? duck : walk;
			}
            break;
		case p1stop:
			state = gameOver ? p1stop : walk;
			break;
        default:
            state = walk;
            break;
    }
    
    switch (state) { //actions
        case walk:
            P1position = 18;
            duration = 0;
            break;
        case jump:
            P1position = 2;
            duration++;
            break;
        case duck:
            P1position = 34;
            break;
		case p1stop:
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
			if (gameOver){
				state = p2stop;
			}
            else if (charge < 7){
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
        case fireMid:;
        case fireBot:
		case p2stop:
			state = gameOver ? p2stop : idle;
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
		case p2stop:
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
            state = gameOver ? stop : move;
            break;
		case stop:
			state = gameOver ? stop : start;
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
				//deleting after it moves offscreen
                if (things[j] % 16 == 0){
                    for (unsigned char k = j; k < numThings; k++){
                        things[k] = things[k+1];
                    }
                    numThings--;
                    j--;
                }                    
            }
            break;
		case stop:
			break;
        default:
            break;
    }
    return state;
}

int tick_Game (int state) {
	switch (state) { //transitions
		case init:
			state = play;
			break;
		case play:
			state = gameOver ? result : play;
			break;
		case result:
			state = gameOver ? result : init;
			break;
		default:
			state = init;
			break;
	}
	
	switch (state) { //actions
		case init:
			score = 0;
			gameOver = 0;
			break;
		case play:
			for (unsigned char j = 0; j < numThings; j++){
				unsigned char loc = things[j];
				//collision
				if (things[j] > 32){
					loc -= 16;
				}
				if (P1position == loc || P1position == things[j]){
					gameOver = 1;
                    things[j]++;
				}
				//score
				else if ((loc-2) % 16 == 0){
					score++;
				}
			}
			break;
		case result:
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
			LCD_DisplayString(1, "START GAME");
            break;
        case display:
            LCD_DisplayString(1, background);
			//objects
            for (unsigned char j = 0; j < numThings; j++){
				if (things[j] < 17){
					LCD_Cursor(things[j]);
					LCD_WriteData(4);
				}
				else if (things[j] < 33){
					LCD_Cursor(things[j]-16);
					LCD_WriteData(5);
					LCD_Cursor(things[j]);
					LCD_WriteData(6);
				}
				else {
					LCD_Cursor(things[j]-16);
					LCD_WriteData(3);
				}
            }
			//player 1
			if (P1position < 33){
				LCD_Cursor(P1position);
				LCD_WriteData(1);
			}
			else {
				LCD_Cursor(P1position-16);
				LCD_WriteData(2);
			}
			//score
			PORTB = score;
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
