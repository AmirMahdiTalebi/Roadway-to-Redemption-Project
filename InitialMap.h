#ifndef INITIAL_MAP_H
#define INITIAL_MAP_H

#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH 937
#define MAP_SIZE 17
#define TILE_SIZE 40

typedef struct kingdom kingdom;
typedef struct village village;

// Global Variables
extern int map[4][MAP_SIZE][MAP_SIZE];
extern int mapHeight, mapWidth;

extern int turn;
extern int villageNumber;
extern int kingdomNumber;

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
//int generate_number();
int initialMapMaker();
int makeKingdom();
int makeVillage();
int makeBarrier();
int dijkstraPath(int source, int id, int size);
int mapDrawer(Texture2D mapTileSet, Texture2D GroundTile, Texture2D Castle, Texture2D House, Texture2D Stone, Font font,Vector2 map0, Vector2 coordination);

#endif // INITIAL_MAP_H