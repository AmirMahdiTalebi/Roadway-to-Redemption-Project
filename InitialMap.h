#ifndef INITIAL_MAP_H
#define INITIAL_MAP_H

#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH 1237
#define MAP_SIZE 17
#define TILE_SIZE 40

typedef struct kingdom kingdom;
typedef struct village village;

// Global Variables
extern int map[2][MAP_SIZE][MAP_SIZE];
extern int mapHeight, mapWidth, turn, kingdomNumber, villageNumber, neededSoldier;
extern int list[MAP_SIZE*MAP_SIZE][5];
extern int mode;

extern Vector2 mousePosition;
extern Vector2 coordination;

extern Color transparentWhite;
extern Color transparentGreen;
extern Color transparentRed;

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

extern village villages[30];
extern kingdom kingdoms[5];

// Functions' Prototypes
//int generate_number();
int initialMapMaker();
int makeKingdom();
int makeVillage();
int makeBarrier();
int dijkstraPath(int source, int id, int size);
int mapDrawer(Texture2D mapTileSet, Texture2D GroundTile, Texture2D Castle, Texture2D House, Texture2D Stone, Font font,Vector2 map0);
int checkNeighbors(int x, int y, Vector2 map0);

#endif // INITIAL_MAP_H