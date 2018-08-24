/*
 * Luis Enriquez-Contreras
 * Victor Ho 
 * Lab Section: 21
 * Assignment: Lab 10 Exercise 3
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

enum SM1_States{lcdNum};

int SMTick1(int state){
	unsigned char getNum;
	getNum = GetKeypadKey();
	switch(state){
		case lcdNum:
			switch (getNum) {
				case '\0': break; 
				case '1': tmpB = 0x01;
				LCD_Cursor(1);
				LCD_WriteData(tmpB + '0'); break; 
				case '2': tmpB = 0x02;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + '0'); break;
				case '3': tmpB = 0x03;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + '0'); break;
				case '4': tmpB = 0x04; 
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + '0'); break;
				case '5': tmpB = 0x05;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + '0'); break;
				case '6': tmpB = 0x06;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + '0'); break;
				case '7': tmpB = 0x07;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + '0'); break;
				case '8': tmpB = 0x08;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + '0'); break;
				case '9': tmpB = 0x09;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + '0'); break;
				case 'A': tmpB = 0x0A;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + 0x37); break;
				case 'B': tmpB = 0x0B;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + 0x37); break;
				case 'C': tmpB = 0x0C;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + 0x37); break;
				case 'D': tmpB = 0x0D;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + 0x37); break;
				case '*': tmpB = 0x0E;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + 0x1C); break;
				case '0': tmpB = 0x00;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + '0'); break;
				case '#': tmpB = 0x0F;
				LCD_Cursor(1); 
				LCD_WriteData(tmpB + 0x14); break;
				default: tmpB = 0x1B; break; // Should never occur. Middle LED off.
			}
			state = lcdNum;
			PORTB=tmpB;
			break;
		}
		return state;
}


int main()
{
	DDRA = 0xF0; PORTA = 0x0F; 
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; 
	DDRD = 0xFF; PORTD = 0x00;
	// Period for the tasks
	unsigned long int SMTick1_calc = 50;


	// gcd
	unsigned long int tmpGCD = 1;

	// find gcd
	unsigned long int GCD = tmpGCD;


	unsigned long int SMTick1_period = SMTick1_calc;

	//array task
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// task 1
	task1.state = 0;
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &SMTick1;


	// timer 
	TimerSet(GCD);
	TimerOn();
	LCD_init();

	unsigned short i; 
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

	return 0;
}