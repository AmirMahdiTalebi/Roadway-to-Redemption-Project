#ifndef INITIAL_MAP_H
#define INITIAL_MAP_H

#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH 1237
#define MAP_SIZE 17
#define TILE_SIZE 40

typedef struct kingdom kingdom;
typedef struct village village;
typedef struct button button;

// Global Variables
extern int map[2][MAP_SIZE][MAP_SIZE];
extern int mapHeight, mapWidth, turn, kingdomNumber, villageNumber, neededSoldier, opponent, winner;
extern int list[MAP_SIZE*MAP_SIZE][5];
extern int mode;

extern Vector2 mousePosition;
extern Vector2 coordination;

extern Color transparentWhite;
extern Color transparentGreen;
extern Color transparentRed;

extern float buttonsPosY;
extern int action;

struct kingdom {
    int x;
    int y;
    int food;
    int foodX;
    int gold;
    int goldX;
    int worker;
    int soldier;
    int roadLeftover[MAP_SIZE][MAP_SIZE];
    Vector2 road[MAP_SIZE*MAP_SIZE];
    int roadNumber;
    int villageNumber;
    Color color;
    int dead;
};

struct village {
    int x;
    int y;
    int goldX;
    int foodX;
    int path[289];
    int pathNumber;
    int kingdom;
};

struct button {
    Rectangle rect;
    char text[30];
    Color color;
};

extern button buttons[5];
extern village villages[30];
extern kingdom kingdoms[5];

// Functions' Prototypes
//int generate_number();
void initialMapMaker();
void makeKingdom();
void makeVillage();
void makeBarrier();
int dijkstraPath(int source, int id, int size);
void mapDrawer(Texture2D mapTileSet, Texture2D GroundTile, Texture2D Castle, Texture2D House, Texture2D Stone, Font font,Vector2 map0);
int checkNeighbors(int x, int y, Vector2 map0);
int checkForWar(int x, int y, Vector2 map0);
void DeleteKingdom(int id);
void mode0();
void mode1();

#endif // INITIAL_MAP_H