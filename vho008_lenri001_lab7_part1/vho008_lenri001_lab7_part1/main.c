/*
 * Victor Ho
 * Luis Enriquez-Contreras
 * Lab Section: 21
 * Assignment: Lab 7 Exercise 1
 *
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "taskScheduler.h"
#include "ThreeLED.h"
#include "BlinkLED.h"
#include "CombineLED.h"

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

const unsigned char tasksSize = 3;
Task tasks[tasksSize];
unsigned char tasksPeriod = 1000;

void initializeTasks();

int main()
{
	//initialize

	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(tasksPeriod);
	TimerOn();
		
    while (1) {}
}

void initializeTasks(){
	unsigned char index = 0;

	tasks[index].state = -1;
	tasks[index].period = tasksPeriod;
	tasks[index].elapsedTime = 0;
	tasks[index].TickFct = &ThreeLED_tick;
	index++;
	
	tasks[index].state = -1;
	tasks[index].period = tasksPeriod;
	tasks[index].elapsedTime = 0;
	tasks[index].TickFct = &BlinkLED_tick;
	index++;
	
	tasks[index].state = -1;
	tasks[index].period = tasksPeriod;
	tasks[index].elapsedTime = 0;
	tasks[index].TickFct = &CombineLED_tick;
		
}