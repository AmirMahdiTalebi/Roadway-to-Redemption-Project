#ifndef INITIAL_MAP_H
#define INITIAL_MAP_H

#define SCREEN_HEIGHT 896
#define SCREEN_WIDTH 960
#define MAP_SIZE 17
#define TILE_SIZE 32

// Global Variables
extern int map[4][MAP_SIZE][MAP_SIZE];
extern int mapHeight, mapWidth;

// Functions' Prototypes
int initialMapMaker();
int makeKingdom();
int makeVillage();
int makeBarrier();
int mapDrawer(Texture2D mapTileSet, Vector2 map0, Vector2 coordination);

#endif // INITIAL_MAP_H