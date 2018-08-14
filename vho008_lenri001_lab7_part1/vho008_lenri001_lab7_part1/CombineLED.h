//CombineLED.h

enum CombineLED_States {start, display};

int CombineLED_tick(int state){
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

