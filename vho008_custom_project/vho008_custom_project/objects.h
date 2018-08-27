//objects.h
#include "objects.c"

enum tick_Player1 {walk, jump, duck};
int tick_Player1 (int state);

enum tick_Player2 {idle, fireTop, fireMid, fireBot};
int tick_Player2 (int state);

enum tick_Stuff {start, move};
int tick_Stuff (int state);