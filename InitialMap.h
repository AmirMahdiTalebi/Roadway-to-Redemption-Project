#ifndef INITIAL_MAP_H
#define INITIAL_MAP_H

#define SCREEN_HEIGHT 896
#define SCREEN_WIDTH 960
#define MAP_SIZE 17
#define TILE_SIZE 32

typedef struct kingdom kingdom;
typedef struct village village;

// Global Variables
extern int map[4][MAP_SIZE][MAP_SIZE];
extern int mapHeight, mapWidth;
extern int turn;
struct village {
    int x;
    int y;
    int goldX;
    int foodX;
    int path[289];
    int pathNumber;
    int king;
};


struct kingdom {
    int x;
    int y;
    int foodX;
    int goldX;
};

extern village villages[30];
extern kingdom kingdoms[5];


// Functions' Prototypes
int initialMapMaker();
int makeKingdom();
int makeVillage();
int makeBarrier();
int dijkstraPath(int source, int id, int size);
int mapDrawer(Texture2D mapTileSet, Vector2 map0, Vector2 coordination);

#endif // INITIAL_MAP_H