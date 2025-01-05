//
// Created by ASUS on 12/31/2024.
//

#ifndef INITIALMAP_C_WAR_H
#define INITIALMAP_C_WAR_H

#include "Map.h"
#include "raylib.h"

void normalWar(int warWinner, int warLoser, int warType, Vector2 loserV);
void loseVillage(int villageID, int loser);
void conquerVillage(int villageID, int conqueror);
int dijkstraForEditingRoads(int source, int id, int size);
int checkForWar(int x, int y);
void DeleteKingdom(int id);

#endif //INITIALMAP_C_WAR_H
