#include <stdio.h>
#include "raylib.h"
#include "InitialGame.h"

typedef struct kingdom kingdom;
typedef struct village village;

// Global Variables
int map[2][MAP_SIZE][MAP_SIZE];
int mapHeight = 0;
int mapWidth = 0;
int winner;
int turn = 1, kingdomNumber, villageNumber, opponent;
int opponentX, opponentY;
int list[MAP_SIZE*MAP_SIZE][5];
int mode = 0, isPlayingWithMonte = 0, iterations = 0;
int MakeRoad = 0, roadX, roadY;
int animation=0, toBeDeleted = 0;

Vector2 mousePosition;
Vector2 coordination;
Vector2 manPos;
Vector2 map0;

Color transparentWhite = (Color){255, 255, 255, 60};
Color transparentGreen = (Color){37, 204, 81, 90};
Color transparentRed = (Color){255, 0,0,60};

float buttonsPosY;
int action;

button buttons[5];
village villages[30] = {0};
kingdom kingdoms[6] = {0};

int generate_number() {
    float probs[4] = {.65, .25, .05, .05};

    float randValue = GetRandomValue(0, 100) / 100.0f;

    if (randValue < probs[0]) {
        return 1;
    } else if (randValue < probs[0] + probs[1]) {
        return 2;
    } else if (randValue < probs[0] + probs[1] + probs[2]) {
        return 3;
    } else {
        return 4;
    }
}

void initialMapMaker() {
    // Getting map's width and height
    printf("Enter the map height:");
    scanf("%d", &mapHeight);
    while(mapHeight<3 || mapHeight>MAP_SIZE) {
        printf("Map height is invalid.\nEnter the map height:");
        scanf("%d", &mapHeight);
    }

    printf("Enter the map width:");
    scanf("%d", &mapWidth);
    while(mapWidth<3 || mapWidth>MAP_SIZE) {
        printf("Map width is invalid.\nEnter the map width:");
        scanf("%d", &mapWidth);
    }

    // Initializing map's values
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            map[0][i][j] = generate_number();
            map[1][i][j] = 0;
        }
    }
}

void makeKingdom() {
    int x, y;
    printf("Enter the number of kingdoms:");
    scanf("%d", &kingdomNumber);
    while(kingdomNumber<1 || kingdomNumber>4) {
        printf("Too many kingdoms\nEnter the number of kingdoms:");
        scanf("%d", &kingdomNumber);
    }
    if (kingdomNumber == 2) {
        char gameMode;
        printf("Do you want to play with computer? (Y,N)\n");
        scanf("%c", &gameMode);
        while (gameMode != 'y' && gameMode != 'Y' && gameMode != 'N' && gameMode != 'n') {
            scanf("%c", &gameMode);
        }
        if (gameMode == 'Y' || gameMode == 'y') {
            isPlayingWithMonte = 1;
            int level;
            printf("Choose the level of your opponent (1-5)\n");
            scanf("%d", &level);
            while(level < 1 || level > 5) {
                printf("Wrong Input!\nChoose the level of your opponent (1-5)\n ");
                scanf("%d", &level);
            }
            iterations = level * 200;
            printf("First kingdom is yours and second is for computer.\n");
        }
    }
    kingdoms[0].dead = 1;
    for (int i = 1; i <= kingdomNumber; ++i) {
        printf("Enter x and y for the kingdom No.%d:\n", i);
        scanf("%d %d", &x, &y);
        while(x<=0 || x>mapWidth || y<=0 || y>mapHeight) {
            printf("x and y are invalid.\nEnter x and y for the kingdom No.%d:", i);
            scanf("%d %d", &x, &y);
        }
        while(map[0][x-1][y-1] <= 0) {
            printf("There is already something else there.\nEnter x and y for the kingdom No.%d:", i);
            scanf("%d %d", &x, &y);
        }
        //x and y start from 1
        x--;
        y--;
        kingdoms[i].x = x;
        kingdoms[i].y = y;
        map[0][x][y] = -1; // -1 is the code for kingdoms.
        map[1][x][y] = i; // I saved Kingdoms' IDs in z=1.
        kingdoms[i].gold = 5;
        kingdoms[i].food = kingdoms[i].foodX = kingdoms[i].soldier = kingdoms[i].villageNumber = kingdoms[i].dead = 0;
        kingdoms[i].goldX = kingdoms[i].worker = 1;
        for (int mapX = 0; mapX < mapWidth; ++mapX) {
            for (int mapY = 0; mapY < mapHeight; ++mapY) {
                kingdoms[i].roadLeftover[mapX][mapY] = map[0][mapX][mapY];
            }
        }
    }
}

void makeVillage() {
    int x, y;
    printf("Enter the number of villages:");
    scanf("%d", &villageNumber);
    for (int i = 0; i < villageNumber; ++i) {
        printf("Enter the info for the village No.%d:\n", i + 1);
        printf("x and y:");
        scanf("%d %d", &x, &y);
        while(x<=0 || x>mapWidth || y<=0 || y>mapHeight) {
            printf("X and y are invalid.\nx and y:");
            scanf("%d %d", &x, &y);
        }
        while(map[0][x-1][y-1] <= 0) {
            printf("There is already something else there.\nx and y:");
            scanf("%d %d", &x, &y);
        }

        // x and y start from 1
        x--;
        y--;
        map[0][x][y] = -2; // -2 is the code for villages.
        map[1][x][y] = i; // I saved villages' IDs in z=1.
        villages[i].x = x;
        villages[i].y = y;

        printf("goldX:");
        scanf("%d", &villages[i].goldX);
        while(villages[i].goldX<0) {
            printf("Number is invalid.\ngoldX:");
            scanf("%d", &villages[i].goldX);
        }

        printf("foodX:");
        scanf("%d", &villages[i].foodX);
        while(villages[i].foodX<0) {
            printf("Number is invalid.\nfoodX:");
            scanf("%d", &villages[i].foodX);
        }
    }
}

void makeBarrier() {
    int n, x, y;
    printf("Enter the number of barriers:");
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        printf("Enter the x and y of the barrier:");
        scanf("%d %d", &x, &y);
        while(x<=0 || x>mapWidth || y<=0 || y>mapHeight) {
            printf("X and y are invalid.\nEnter the x and y of the barrier:");
            scanf("%d %d", &x, &y);
        }
        while(map[0][x-1][y-1] <= 0) {
            printf("There is already something else there.\nEnter the x and y of the barrier:");
            scanf("%d %d", &x, &y);
        }
        //x and y start from 1
        x--;
        y--;
        map[0][x][y] = -3; // -3 is the code for barriers.
    }
}

void mapDrawer(Texture2D mapTileSet, Texture2D GroundTile, Texture2D Castle, Texture2D House,Texture2D Stone, Texture2D quoteBox, Font font, Font myFont) {
    DrawTexture(mapTileSet, 0,0, WHITE);
    for(int i=0; i<mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {

            DrawTexture(GroundTile, i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, WHITE);
            DrawRectangleLines(i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE,
                               (Color) {150, 75, 0, 100});

            switch (map[0][i][j]) {

                case -1: // -1 is the code for kingdoms.
                    DrawTexture(Castle, i * TILE_SIZE + map0.x, (j - 0.5) * TILE_SIZE + map0.y,
                                kingdoms[map[1][i][j]].color);
                    break;

                case -2: // -2 is the code for villages.
                    DrawTexture(House, i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y,
                                kingdoms[villages[map[1][i][j]].kingdom].color);
                    break;

                case -3: // -3 is the code for barriers.
                    DrawTexture(Stone, i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, WHITE);
                    break;

                default: // It is used for roadways' numbers.
                    DrawRectangleLines(i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, BROWN);

                    if (map[1][i][j] > 0) {
                        DrawTexture(GroundTile, i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y,
                                    kingdoms[map[1][i][j]].color);
                    } else {
                        // Writing roadways' numbers on the tile
                        char innerNum[2];
                        sprintf(innerNum, "%d", kingdoms[turn].roadLeftover[i][j]);
                        DrawTextEx(font, innerNum,
                                   (Vector2) {(i + .4) * TILE_SIZE + map0.x, (j + .15) * TILE_SIZE + map0.y}, 30, 1,
                                   (Color) {150, 75, 0, 200});
                    }
            }

            // Hover Effect
            if (coordination.x == i && coordination.y == j) {
                DrawRectangle(i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentWhite);
            }
        }
    }

    //Villages' Information
    for(int i=0; i<mapWidth; i++) {
        for(int j=0; j<mapHeight; j++) {
            if(coordination.x==i && coordination.y==j && map[0][i][j]==-2) {
                //show the shortest path
                int id = map[1][i][j];
                for (int k = 1; k < villages[id].pathNumber; k++) {
                    DrawRectangle((villages[id].path[k] % mapWidth) * TILE_SIZE + map0.x,
                                  (villages[id].path[k] / mapWidth) * TILE_SIZE + map0.y,
                                  TILE_SIZE, TILE_SIZE, transparentWhite);
                }
//                village info
                DrawTexture(quoteBox, (i-1) * TILE_SIZE + map0.x, (j-1) * TILE_SIZE + map0.y, WHITE);
                char goldFood[30];
                sprintf(goldFood, "%d gold\n%d food", villages[id].goldX, villages[id].foodX);
                DrawTextEx(myFont, goldFood, (Vector2) {(i-.85) * TILE_SIZE + map0.x, (j-.85) * TILE_SIZE + map0.y},
                           20, 1, Fade(kingdoms[villages[id].kingdom].color, .5));
            }
        }
    }
}

void SaveGame(gameState* game) {
    game->kingdomNumber = kingdomNumber;
    for (int i = 0; i <= kingdomNumber; ++i) {
        game->kingdom[i] = kingdoms[i];
    }
    game->villageNumber = villageNumber;
    for (int i = 0; i < villageNumber; ++i) {
        game->villages[i] = villages[i];
    }
    game->mapWidth = mapWidth;
    game->mapHeight = mapHeight;
    for (int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; ++j) {
            for (int k = 0; k < 2; ++k) {
                game->map[k][j][i] = map[k][j][i];
            }
        }
    }
    game-> winner = winner;
    if (mode==3)
        game->end = 1;
    else
        game->end = 0;
    game->turn = turn;
    game->isPlayingWithMonte = isPlayingWithMonte;
}

void LoadGame(gameState* game) {
    kingdomNumber = game->kingdomNumber;
    for (int i = 0; i <= kingdomNumber; ++i) {
        kingdoms[i] = game->kingdom[i];
    }
    villageNumber = game->villageNumber;
    for (int i = 0; i < villageNumber; ++i) {
        villages[i] = game->villages[i];
    }
    mapWidth = game->mapWidth;
    mapHeight = game->mapHeight;
    for (int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; ++j) {
            for (int k = 0; k < 2; ++k) {
                map[k][j][i] = game->map[k][j][i];
            }
        }

        winner = game->winner;
        if (game->end == 1)
            mode = 3;
        else
            mode = 0;
        turn = game->turn;
    }
    isPlayingWithMonte = game->isPlayingWithMonte;
}

