#ifndef INITIAL_MAP_H
#define INITIAL_MAP_H

#include "War.h"
#include "raylib.h"
#include "monteCarlo.h"


#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH 1237
#define MAP_SIZE 17
#define TILE_SIZE 40

extern int animation;
extern int toBeDeleted;
#define MAN_COUNT 6
#define MAN_TIME 0.1
#define FLAME_COUNT 28
#define FLAME_TIME 0.03
#define FLAME_WIDTH 68
#define FLAME_HEIGHT 60
extern Vector2 manPos;

// Global Variables
extern int map[2][MAP_SIZE][MAP_SIZE];
extern int mapHeight, mapWidth, turn, kingdomNumber, villageNumber, opponent, winner, dijkstraX, dijkstraY;
extern int list[MAP_SIZE*MAP_SIZE][5];
extern int mode, monteCarlo;
extern int MakeRoad, roadX, roadY;

extern Vector2 mousePosition;
extern Vector2 coordination;
extern Vector2 map0;

extern Color transparentWhite;
extern Color transparentGreen;
extern Color transparentRed;

extern float buttonsPosY;
extern int action;

typedef struct kingdom {
    int x;
    int y;
    int food;
    int foodX;
    int gold;
    int goldX;
    int worker;
    int soldier;
    int roadLeftover[MAP_SIZE][MAP_SIZE];
    int roadList[MAP_SIZE*MAP_SIZE][5];
    Vector2 road[MAP_SIZE*MAP_SIZE];
    int roadNumber;
    int path[289];
    int pathNumber;
    int villageNumber;
    Color color;
    int dead;
    Vector2 available[MAP_SIZE*MAP_SIZE];
    int availableNumber;
} kingdom;

typedef struct village {
    int x;
    int y;
    int goldX;
    int foodX;
    int path[289];
    int pathNumber;
    int kingdom;
} village;

typedef struct gameState {
    int kingdomNumber;
    kingdom kingdom[6];
    int villageNumber;
    village villages[30];
    int mapWidth, mapHeight;
    int map[2][MAP_SIZE][MAP_SIZE];
    int turn;
    int winner;
    int end;
}gameState;

typedef struct button {
    Rectangle rect;
    char text[30];
    Color color;
} button;

extern button buttons[5];
extern village villages[30];
extern kingdom kingdoms[6];

// Functions' Prototypes
//int generate_number();
void initialMapMaker();
void makeKingdom();
void makeVillage();
void makeBarrier();
int dijkstraPath(int source, int id, int size);
void mapDrawer(Texture2D mapTileSet, Texture2D GroundTile, Texture2D Castle, Texture2D House, Texture2D Stone, Font font);
int checkNeighbors(int x, int y);
void RoadMaker();
void mode0();
void mode1();
void SaveGame(gameState* game);
void LoadGame (gameState* game);

#endif // INITIAL_MAP_H