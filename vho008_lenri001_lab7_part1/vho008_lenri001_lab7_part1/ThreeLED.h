//ThreeLED.h

enum ThreeLED_States {start, light0, light1, light2};

int ThreeLED_tick(int state){
	switch (state){ //transitions
		case start:
			state = light0;
			break;
		case light0:
			state = light1;
			break;
		case light1:
			state = light2;
			break;
		case light2:
			state = light0;
			break;
		default:
			state = start;
			break;
	}
	
	switch (state){ //actions
		case start:
			outputThree = 0x00;
			break;
		case light0:
			outputThree = 0x01;
			break;
		case light1:
			outputThree = 0x02;
			break;
		case light2:
			outputThree = 0x04;
			break;
		default:
			break;
	}
	return state;
}