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

char customGhost[] = {
	0x0E,
	0x1F,
	0x15,
	0x1F,
	0x1F,
	0x15,
	0x00,
	0x00
};

char customGhostTop[] = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0E,
	0x15,
	0x1F
};

char customGhostBot[] = {
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
unsigned char things[9] = {0, 0, 0, 0, 0, 0, 0, 0, NULL};
unsigned char gameOver;
unsigned char score;
unsigned char highScore;
unsigned char P1position;
unsigned char P2position;
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

enum tick_Player2 {idle, moveUp, moveUpWait, fire, moveDown, moveDownWait, p2stop};
int tick_Player2 (int state);

enum tick_Stuff {start, move, stop};
int tick_Stuff (int state);

enum tick_Game {init, play, finish};
int tick_Game (int state);

enum tick_Display {clear, display, result};
int tick_Display (int state);

//timer
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tasksPeriod = 50;
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
	tasks[i].period = 50;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &tick_Player1;
	i++;
	tasks[i].state = -1;
	tasks[i].period = 50;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &tick_Player2;	
    i++;
	tasks[i].state = -1;
	tasks[i].period = 400;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &tick_Stuff;
    i++;
	tasks[i].state = -1;
	tasks[i].period = 400;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &tick_Game;
	i++;
    tasks[i].state = -1;
    tasks[i].period = 100;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &tick_Display;
    i++;
    
	//declare ports
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFE; PORTD = 0x01;
	
	//initialize
    TimerSet(tasksPeriod);
    TimerOn();
	LCD_init();
	LCD_WriteCommand(0x0C);
    LCD_Custom_Char(1, customChar);
	LCD_Custom_Char(2, customCharDuck);
	LCD_Custom_Char(3, customGhost);
	LCD_Custom_Char(4, customGhostTop);
	LCD_Custom_Char(5, customGhostBot);	
	LCD_WriteCommand(0x80);
	
    gameOver = 0;
	
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
            else if (duration < 12){
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
			else if (A1) {
				state = duck;
			}
            else {
                state = walk;
            }
            break;
		case p1stop:
			state = gameOver ? p1stop : walk;
			break;
        default:
            state = walk;
			P1position = 18;
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
    if (charge < 22){
        PORTD = 0x00;
    }             
    else {
        PORTD = 0x01;
    }
    switch (state) {//transitions
        case idle:
			if (gameOver){
				state = p2stop;
			}
            else if (charge < 22){
                state = idle;
            }
            else {
                if (A2){
                state = moveUp;
                }
                else if (A3){
                    state = fire;
                }
                else if (A4){
                    state = moveDown;
                }
                else {
                    state = idle;
                }
            }                
            break;
        case moveUp:
			state = gameOver ? p2stop : moveUpWait;
			break;
        case moveUpWait:
			if (!A2){
				state = gameOver ? p2stop : idle;
			}
			else {
				state = gameOver ? p2stop : moveUpWait;
			}
			break;
		case fire:
			state = gameOver ? p2stop : idle;
			break;
        case moveDown:
			state = gameOver ? p2stop : moveDownWait;
			break;
		case moveDownWait:
			if (!A4){
				state = gameOver ? p2stop : idle;
			}
			else {
				state = gameOver ? p2stop : moveDownWait;
			}
			break;
		case p2stop:
			state = gameOver ? p2stop : idle;
			break;
        default:
            state = idle;
            charge = 0;
			P2position = 16;
            break;
    }
    
    switch (state) { //actions
        case idle:
            if (charge < 22){
                charge++;
            }
            break;
        case moveUp:
            if (P2position > 16){
				P2position -= 16;
			}
            break;
		case moveUpWait:
			break;
        case fire:
            charge = 0;
            things[numThings] = P2position;
            numThings++;
            break;
        case moveDown:
            if (P2position < 48){
				P2position += 16;
			}
            break;
		case moveDownWait:
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
                    LCD_Cursor(1);
                    LCD_WriteData(' ');
                    LCD_Cursor(17);
                    LCD_WriteData('_');
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
			state = gameOver ? finish : play;
			break;
		case finish:
			state = gameOver ? finish : init;
			break;
		default:
			state = init;
			break;
	}
	
	switch (state) { //actions
		case init:
			score = 0;
			break;
		case play:
			for (unsigned char j = 0; j < numThings; j++){
				unsigned char loc = things[j];
				//collision
				if (P1position == loc || P1position == things[j]){
					gameOver = 1;
                    things[j]++;
				}
				else if (things[j] > 32 && P1position == (things[j]-16)){
					gameOver = 1;
					things[j]++;				
				}
				//score
				else if ((loc-2) % 16 == 0){
					score++;
				}
			}
			break;
		case finish:
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
            state = gameOver ? result : display;
            break;
		case result:
			state = gameOver ? result : clear;
			break;
        default:
            state = clear;
            break;
    }
    
    switch (state) { //actions
        case clear:
        	LCD_ClearScreen();
			LCD_DisplayString(1, "START GAME");
        	LCD_DisplayString(1, background);
            LCD_Cursor(P1position);
            LCD_WriteData(1);
            LCD_Cursor(P2position);
            LCD_WriteData(3);
			highScore = eeprom_read_byte(1);
            break;
        case display:
        	//player 1
        	if (P1position < 17){
            	LCD_Cursor(P1position+16);
            	LCD_WriteData('_');
            			
            	LCD_Cursor(P1position);
            	LCD_WriteData(1);
        	}
        	else if (P1position < 33){
            	LCD_Cursor(P1position-16);
            	LCD_WriteData(' ');
            			
            	LCD_Cursor(P1position);
            	LCD_WriteData(1);
        	}
        	else {
            	LCD_Cursor(P1position-32);
            	LCD_WriteData(' ');
            			
            	LCD_Cursor(P1position-16);
            	LCD_WriteData(2);
        	}
        			
        	//player 2
        	if (P2position < 17){
            	LCD_Cursor(P2position+16);
            	LCD_WriteData('_');
            			
            	LCD_Cursor(P2position);
            	LCD_WriteData(3);
        	}
        	else if (P2position < 33){
            	LCD_Cursor(P2position-16);
            	LCD_WriteData(4);
            	LCD_Cursor(P2position);
            	LCD_WriteData(5);
        	}
        	else {
            	LCD_Cursor(P2position-32);
            	LCD_WriteData(' ');
            			
            	LCD_Cursor(P2position-16);
            	LCD_WriteData(3);
        	}
			//objects
            for (unsigned char j = 0; j < numThings; j++){
				if (things[j] < 17){
					LCD_Cursor(things[j]+1);
					LCD_WriteData(' ');
					
					LCD_Cursor(things[j]);
					LCD_WriteData(3);
				}
				else if (things[j] < 33){
					LCD_Cursor(things[j]-15);
					LCD_WriteData(' ');
					LCD_Cursor(things[j]+1);
					LCD_WriteData('_');
					
					LCD_Cursor(things[j]-16);
					LCD_WriteData(4);
					LCD_Cursor(things[j]);
					LCD_WriteData(5);
				}
				else {
					LCD_Cursor(things[j]-15);
					LCD_WriteData('_');
					
					LCD_Cursor(things[j]-16);
					LCD_WriteData(3);
				}
            }   
			//score
			PORTB = score;
            break;
		case result:
			if (score > highScore){
				highScore = score;
			}
			eeprom_write_byte(1, highScore);
			gameOver = 0;
            
			break;
        default:
            break;
    }
    return state;
}

//--------------------------------------------------------------------------------------
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
