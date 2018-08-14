//BlinkLED.h

enum BlinkLED_States {start, light3On, light3Off};

int BlinkLED_tick(int state){
	switch (state){ //transitions
		case start:
			break;
		default:
			state = start;
			break;
	}
	
	switch (state){ //actions
		case start:
			break;
		default:
			break;
	}
	return state;
}