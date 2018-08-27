#include <avr/io.h>
#include <scheduler.h>
#include <io.c>
#include <keypad.h>
#include <timer.h>


enum SM1_States {SM1_Start,SM1_Init,SM1_Wait};
enum SM2_States{SM2_Start,SM2_Init,SM2_Wait};
unsigned char i;

int SMTick1(int state){
	switch(state){
		case SM1_Start:
		state = SM1_Init;
		break;
		case SM1_Init:
		state = SM1_Wait;
		break;
		case SM1_Wait:
		break;
		default:
		state = SM1_Start;
		break;
	}
	switch(state){
		case SM1_Wait:
		i = GetKeypadKey();
		break;
	}
	return state;
}
int SMTick2(int state){
	static unsigned char j;
	static unsigned char k;
	switch(state){
		case SM2_Start:
		state = SM2_Init;
		break;
		case SM2_Init:
		state = SM2_Wait;
		LCD_Cursor(0);
		j = 0;
		break;
		case SM2_Wait:
		break;
		default:
		state = SM2_Start;
		break;
	}
	switch(state){
		case SM2_Wait:
		if(j >= 16){
			state = SM1_Init;
		}
		else if(i != '\0' && k == '\0'){
			if(j == 0 ){
				LCD_Cursor(1);
			}
			LCD_WriteData(i);
			j++;
			k = i;
		}
		else{
			k = i;
		}
		break;
	}
	return state;
}
int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;

	unsigned long int SMTick1_calc = 50;
	unsigned long int SMTick2_calc = 500;

	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	
	//Declare an array of tasks
	static task task1, task2;
	task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = SM1_Start;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.

	// Task 2
	task2.state = SM2_Start;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &SMTick2;//Function pointer for the tick.
	
	LCD_init();
	LCD_ClearScreen();
	LCD_DisplayString(1,"Congratulations!");
	
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
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