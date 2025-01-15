#ifndef INITIALMAP_C_WAR_H
#define INITIALMAP_C_WAR_H

#include "InitialGame.h"
#include "raylib.h"
#include "monteCarlo.h"
#include "GameUpdate.h"

void normalWar(int warWinner, int warLoser, int warType, Vector2 loserV);
void loseVillage(int villageID, int loser);
void conquerVillage(int villageID, int conqueror);
int dijkstraForEditingRoads(int source, int id, int size);
int checkForWar(int x, int y);
void DeleteKingdom(int id);

#endif //INITIALMAP_C_WAR_H
