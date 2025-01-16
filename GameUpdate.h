#ifndef MAIN_C_GAMEUPDATE_H
#define MAIN_C_GAMEUPDATE_H

#include "War.h"
#include "InitialGame.h"
#include "monteCarlo.h"

int dijkstraVillage(int source, int id, int size);
int checkNeighbors(int x, int y);
void RoadMaker();
void mode0();
void mode1();
void mode3();

#endif //MAIN_C_GAMEUPDATE_H
