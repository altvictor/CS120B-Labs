//BlinkLED.h

enum BlinkLED_States {start, lightOn, lightOff};

int BlinkLED_tick(int state){
	switch (state){ //transitions
		case start:
			state = lightOn;
			break;
		case lightOn:
			state = lightOff;
			break;
		case lightOff:
			state = lightOn;
			break;
		default:
			state = start;
			break;
	}
	
	switch (state){ //actions
		case start:
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