/*	Melissa Quero-Gopar mquer001@ucr.edu and Danielle Yee
 *	Lab Section: 027
 *	Assignment: Lab4  Exercise2 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

// each time you set a variable, your giving it the capability to be 1, so this is how ur able to say that both the button are 
// pressed. then you set it equal to what ever pin bit that hasnt been initialized, so it could have the value of 0, which is your
// goal of resetting it. 

enum SM_States{Init,Start, Base, But1, But2, But3, Reset, LetGo} state;

unsigned char buttA = 0x00;
unsigned char buttB = 0x00;
unsigned char output = 0x00;

void SMtick(){

//led1 = !PINB & 0x01;
//led2 = !PINB & 0x02
buttA = (~PINB) & 0x01;
buttB = (~PINB) & 0x02;
		switch(state)  //Cases For Transitions
		{
			case Init:
			state = Start;
			break;
			
			case Start:
			state = Base;
			break;
			
			case Base:
			if(buttA && !buttB)
			{
				state = But1;
			}
			else if(!buttA && !buttB)
			{
				state = Base;
			}			
			else if(buttA && buttB)
			{
				state = Reset;
			}
			
			case But1:
			if(buttA && !buttB)
			{
				state = But1;
			}
			else{
				state = LetGo;
			}
			break;
			
			case But2:
			if(buttB && !buttA)
			{
				state = But2;
			}
			else{
				state = LetGo;
			}
			break;
			
			case Reset:
			if(buttA && buttB)
			{
				state = Reset;
			}
			else{
				state = LetGo;
			}
			break;
			
			case LetGo:
			if(buttA || buttB)
			{
				state = LetGo;
			}
	//		else if (!(buttA || buttB)){
		else{
				state = Base;
			}
			break;
			
			default:
			state = Start;
			break;
			
		}
		switch(state)  //Cases For Actions
		{
			case Init:
			break;
			
			case Start:
			output = 0x07;
			PORTA = output;
			break;
			
			case Base:
			break;
			
			case But1:
			if(output < 0x09)
			{
				output = output + 1;
				PORTA = output;
			}
			else{
				//output;
			PORTA = output;
			}
			break;
			
			case But2:
			if(output > 0x00)
			{
				output = output-1;
				PORTA = output;
			}
			else{
				output;
			PORTA = output;
			}
			break;
			
			case Reset:
			PORTA = 0x00;
			break;
			
			case LetGo:
			break;
					
			default:
			break;
	 
		}
}
int main(void)
{
	PORTA = 0x00; DDRA = 0xFF;
	PORTB = 0xFF; DDRB = 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		SMtick();
    }

}