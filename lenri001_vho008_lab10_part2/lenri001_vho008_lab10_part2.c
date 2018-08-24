/*
 * Luis Enriquez-Contreras
 * Victor Ho 
 * Lab Section: 21
 * Assignment: Lab 10 Exercise 2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include <stdio.h>
#include "io.c"
#include "io.h"
#include "keypad.h"
#include "scheduler.h"

unsigned char tmpB = 0x00;
const unsigned char cstring[44]={' ',' ',' ',' ','C','S','1','2','0','B',' ','i','s',' ','L','e','g','e','n','d','.','.','.','w','a','i','t',' ','f','o','r',' ','i','t',' ','D','A','R','Y','!',' ',' ',' ','  '};

enum SM1_States{SM1_output};
unsigned char countScroll = 1;


int SMTick1(int state){
	switch(state){
		case SM1_output:
		for(int j = 1; j <= 16; j++){
			LCD_Cursor(j);
			LCD_WriteData(cstring[countScroll+j-2]);
			if(countScroll+j+1 == 46){
				countScroll = 1;
			}
			
		}
		countScroll++;
		
	}
	return state;
}


int main()
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; 
	DDRD = 0xFF; PORTD = 0x00;
	// period
	unsigned long int SMTick1_calc = 250;


	unsigned long int tmpGCD = 1;

	// gcd 
	unsigned long int GCD = tmpGCD;

	// find period
	unsigned long int SMTick1_period = SMTick1_calc;

	//tasks setup
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// task 1
	task1.state = 0;
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &SMTick1;


	// set timer
	TimerSet(GCD);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();
	unsigned short i; // 
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime >= tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}