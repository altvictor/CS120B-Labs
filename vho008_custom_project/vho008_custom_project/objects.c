//objects.c
#include "main.c"

#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)
#define A2 (~PINA & 0x04)
#define A3 (~PINA & 0x08)
#define A4 (~PINA & 0x10)

int tick_Player1 (int state) {
	switch (state) {//transitions
		case walk:
            
			break;
        case jump:
            break;
        case duck:
            break;
		default:
			break;
	}
	
	switch (state) { //actions
		case idle:
            P1position = 17;
		    break;
		case jump:
            P1position = 1;
		    break;
		case duck:
		    break;
		default:
		    break;
	}
}

int tick_Player2 (int state) {
    switch (state) {//transitions
        case start:
            break;
        case idle:
            break;
        case fireTop:
            break;
        case fireMid:
            break;
        case fireBot:
            break;
        default:
            break;
    }
    
    switch (state) { //actions
        case start:
            break;
        case idle:
            break;
        case fireTop:
            break;
        case fireMid:
            break;
        case fireBot:
            break;
        default:
            break;
    }
}

int tick_Stuff (int state) {
    switch (state) {//transitions
        case start:
            break;
        case move:
            break;
        default:
            break;
    }
    
    switch (state) { //actions
        case start:
            break;
        case move:
            break;
        default:
            break;
    }
}