//ThreeLED.h

enum ThreeLED_States {start, light0, light1, light2};

int ThreeLED_tick(int state){
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