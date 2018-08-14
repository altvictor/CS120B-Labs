//CombineLED.h

enum CombineLED_States {start, display};

int CombineLED_tick(int state){
	unsigned char tmpB = 0x00;
	
	switch (state){ //transitions
		case start:
			state = display;
			break;
		case display:
			state = display;
			break;
		default:
			state = start;
			break;
	}
	
	switch (state){ //actions
		case start:
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

