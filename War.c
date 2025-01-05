#include "War.h"
#include  "Map.h"
#include "raylib.h"
#include <stdio.h>

int dijkstraForEditingRoads(int source, int id, int size) {
    int dest = kingdoms[id].y*mapWidth + kingdoms[id].x;
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            if (map[0][i][j] > 0 && kingdoms[id].roadLeftover[i][j] > 0) {
                //anything that can't become a road
                kingdoms[id].roadList[j * mapWidth + i][0] = 2000;
            }
            else {
                kingdoms[id].roadList[j * mapWidth + i][0] = 1;
            }
        }
    }
    kingdoms[id].roadList[source][0] = kingdoms[id].roadList[dest][0] = 0;

    for (int i = 0; i < mapWidth * mapHeight; ++i) {
        for (int j = 1; j < 5; ++j) {
            kingdoms[id].roadList[i][j] = -1;
        }
    }

    //Adding neighbors to the list
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            if ((map[0][i][j] == -2 && villages[map[1][i][j]].kingdom == id) || (map[0][i][j] != -2 && map[1][i][j] == id)) {
                int v = j*mapWidth + i;
                int k = 0;
                if ((v - mapWidth) >= 0 && ((map[0][i][j - 1] != -2 && map[1][i][j - 1] == id) || (map[0][i][j - 1] == -2 && villages[map[1][i][j - 1]].kingdom == id))) {
                    kingdoms[id].roadList[v][1+k] = v - mapWidth;
                    k++;
                }
                if ((v % mapWidth) != 0 && ((map[0][i - 1][j] != -2 && map[1][i - 1][j] == id) || (map[0][i - 1][j] == -2 && villages[map[1][i - 1][j]].kingdom == id))) {
                    kingdoms[id].roadList[v][1+k] = v - 1;
                    k++;
                }
                if ((v + 1) % mapWidth != 0  && ((map[0][i + 1][j] != -2 && map[1][i + 1][j] == id) || (map[0][i + 1][j] == -2 && villages[map[1][i + 1][j]].kingdom == id))) {
                    kingdoms[id].roadList[v][1+k] = v + 1;
                    k++;
                }
                if ((v + mapWidth) < (mapWidth * mapHeight) && ((map[0][i][j + 1] != -2 && map[1][i][j + 1] == id) || (map[0][i][j + 1] == -2 && villages[map[1][i][j + 1]].kingdom == id))) {
                    kingdoms[id].roadList[v][1+k] = v + mapWidth;
                }
            }
        }
    }

    //vars
    int dist[size];
    for (int i = 0; i < size; ++i) {
        dist[i] = 2000;
    }
    int path[size][size];
    for (int i = 0; i < size; ++i) {
        path[i][0] = source;
    }
    int pathNumber[289]= {0};
    int visited[289]= {0};
    dist[source]=0;
    pathNumber[source]=1;
    int current=source, neighbor, minDistance;

    //main algorithm loop
    while(!visited[dest]) {

        //update all neighbors
        for(int i=1; kingdoms[id].roadList[current][i]>=0 && i<5; i++) {
            neighbor=kingdoms[id].roadList[current][i];
            if(!visited[neighbor] && (dist[current] + kingdoms[id].roadList[neighbor][0]) < dist[neighbor] && kingdoms[id].roadList[neighbor][0] != -1) {
                //update distance
                dist[neighbor] = dist[current] + kingdoms[id].roadList[neighbor][0];
                //update path
                pathNumber[neighbor]= pathNumber[current]+1;
                for(int j=0; j<pathNumber[current]; j++) {
                    path[neighbor][j]=path[current][j];
                }
                path[neighbor][pathNumber[neighbor]-1]=neighbor;
            }
        }

        //current is visited
        visited[current] = 1;

        //update current
        minDistance=4000;
        for(int i=0; i<size; i++) {
            if(!visited[i] && dist[i]<minDistance && kingdoms[id].roadList[i][0] != -1) {
                current=i;
                minDistance=dist[i];
            }
        }
    }

    //save the path
    int k;
    for (k = 0; k < pathNumber[dest]-1 && kingdoms[id].roadList[path[dest][k]][3]==-1; k++) {
        kingdoms[id].path[k] = path[dest][k];
    }
    kingdoms[id].pathNumber = k;
    return dist[dest];
}

void dfs(int v, int id, int mark[]) {
    mark[v] = 1;
    for (int i = 1; kingdoms[id].roadList[v][i] != -1 && i < 5; ++i) {
        if (!mark[kingdoms[id].roadList[v][i]]) dfs(kingdoms[id].roadList[v][i], id, mark);
    }
}

void garbageCollector(int id) {
    int mark[MAP_SIZE*MAP_SIZE] = {0};

    for (int i = 0; i < mapWidth * mapHeight; ++i) {
        for (int j = 1; j < 5; ++j) {
            kingdoms[id].roadList[i][j] = -1;
        }
    }

    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            if ((map[0][i][j] == -2 && villages[map[1][i][j]].kingdom == id) || (map[0][i][j] != -2 && map[1][i][j] == id)) {
                int v = j*mapWidth + i;
                int k = 0;
                if ((v - mapWidth) >= 0 && ((map[0][i][j - 1] != -2 && map[1][i][j - 1] == id) || (map[0][i][j - 1] == -2 && villages[map[1][i][j - 1]].kingdom == id))) {
                    kingdoms[id].roadList[v][1+k] = v - mapWidth;
                    k++;
                }
                if ((v % mapWidth) != 0 && ((map[0][i - 1][j] != -2 && map[1][i - 1][j] == id) || (map[0][i - 1][j] == -2 && villages[map[1][i - 1][j]].kingdom == id))) {
                    kingdoms[id].roadList[v][1+k] = v - 1;
                    k++;
                }
                if ((v + 1) % mapWidth != 0  && ((map[0][i + 1][j] != -2 && map[1][i + 1][j] == id) || (map[0][i + 1][j] == -2 && villages[map[1][i + 1][j]].kingdom == id))) {
                    kingdoms[id].roadList[v][1+k] = v + 1;
                    k++;
                }
                if ((v + mapWidth) < (mapWidth * mapHeight) && ((map[0][i][j + 1] != -2 && map[1][i][j + 1] == id) || (map[0][i][j + 1] == -2 && villages[map[1][i][j + 1]].kingdom == id))) {
                    kingdoms[id].roadList[v][1+k] = v + mapWidth;
                }
            }
        }
    }


    int kingdomVertex = kingdoms[id].y * mapWidth + kingdoms[id].x;
    dfs(kingdomVertex, id, mark);

    for (int i = 0; i < villageNumber; ++i) {
        if (villages[i].kingdom==id && !mark[villages[i].y*mapWidth+ villages[i].x])
            loseVillage(i, id);
    }

    for (int i = 0; i < kingdoms[id].roadNumber; ++i) {
        int mapX = (int)kingdoms[id].road[i].x;
        int mapY = (int)kingdoms[id].road[i].y;
        if (!mark[mapY * mapWidth + mapX]) {
            if (map[0][mapX][mapY] > 0) {
                for (int roadID = i; roadID < kingdoms[id].roadNumber ; ++roadID) {
                    kingdoms[id].road[roadID] = kingdoms[id].road[roadID + 1];
                }
                kingdoms[id].roadNumber--;
                i--;
                map[1][mapX][mapY] = 0;
                kingdoms[id].roadLeftover[mapX][mapY] = map[0][mapX][mapY];
            }
        }
    }

}

void conquerVillage(int villageID, int conqueror) {
    villages[villageID].kingdom = conqueror;
    kingdoms[conqueror].villageNumber++;
    kingdoms[conqueror].goldX += villages[villageID].goldX;
    kingdoms[conqueror].foodX += villages[villageID].foodX;
    dijkstraForEditingRoads(villages[villageID].y*mapWidth + villages[villageID].x, turn, mapWidth*mapHeight);
}

void loseVillage(int villageID, int loser) {
    villages[villageID].kingdom = 0;
    kingdoms[loser].villageNumber--;
    kingdoms[loser].goldX -= villages[villageID].goldX;
    kingdoms[loser].foodX -= villages[villageID].foodX;
}

void normalWar(int warWinner, int warLoser, int warType, Vector2 loserV) {
    kingdoms[warWinner].soldier -= kingdoms[warLoser].soldier;
    kingdoms[warLoser].soldier = 0;
    dijkstraForEditingRoads(loserV.y*mapWidth + loserV.x, warLoser, mapWidth * mapHeight);

    if (warType ==1) { //war near a road
        for(int k = 0; k < kingdoms[warLoser].pathNumber; k++) {
            //destroy each road
            int pathX = kingdoms[warLoser].path[k] % mapWidth;
            int pathY = kingdoms[warLoser].path[k] / mapWidth;
            if (k != 0 && map[0][pathX][pathY] == -2) break;
            for (int roadID = 0, sw = 0; roadID < kingdoms[warLoser].roadNumber; ++roadID) {
                if (kingdoms[warLoser].road[roadID].x == pathX && kingdoms[warLoser].road[roadID].y == pathY) sw = 1;
                if (sw) kingdoms[warLoser].road[roadID] = kingdoms[warLoser].road[roadID + 1];
            }

            map[1][pathX][pathY] = 0;
            kingdoms[warLoser].roadNumber--;
            kingdoms[warLoser].roadLeftover[pathX][pathY] = map[0][pathX][pathY];

        }
    }
    else if (warType == 2) { //war near a village
        for(int k = 0; k < kingdoms[warLoser].pathNumber; k++) {
            //destroy each road
            int pathX = kingdoms[warLoser].path[k] % mapWidth;
            int pathY = kingdoms[warLoser].path[k] / mapWidth;
            if (k != 0 && map[0][pathX][pathY] == -2) break;
            for (int roadID = 0, sw = 0; roadID < kingdoms[warLoser].roadNumber; ++roadID) {
                if (kingdoms[warLoser].road[roadID].x == pathX && kingdoms[warLoser].road[roadID].y == pathY) sw = 1;
                if (sw) kingdoms[warLoser].road[roadID] = kingdoms[warLoser].road[roadID + 1];
            }

            if (map[0][pathX][pathY] > 0) map[1][pathX][pathY] = 0;
            kingdoms[warLoser].roadNumber--;
            kingdoms[warLoser].roadLeftover[pathX][pathY] = map[0][pathX][pathY];

        }
        if (warLoser != turn) {
            int villageID = map[1][(int) loserV.x][(int) loserV.y];
            loseVillage(villageID, warLoser);
            conquerVillage(villageID, warWinner);
        }
    }
    garbageCollector(warLoser);
}

int checkForWar(int x, int y) { //War types: 1:war near road, 2:war near village, 3:all-out war
    if (x != 0) {

        if (map[0][x - 1][y] == -1 && map[1][x - 1][y] != turn && !kingdoms[map[1][x - 1][y]].dead) {
            opponent = map[1][x - 1][y];
            dijkstraX = x - 1;
            dijkstraY = y;
            return 3;
        }
        if (map[0][x - 1][y] == -2 && villages[map[1][x - 1][y]].kingdom != 0 &&
            villages[map[1][x - 1][y]].kingdom != turn) {
            opponent = villages[map[1][x - 1][y]].kingdom;
            dijkstraX = x - 1;
            dijkstraY = y;
            return 2;
        }
        if (map[0][x - 1][y] > 0 && map[1][x - 1][y] != 0 && map[1][x - 1][y] != turn) {
            opponent = map[1][x - 1][y];
            dijkstraX = x - 1;
            dijkstraY = y;
            return 1;
        }
    }

    if (y != 0) {

        if (map[0][x][y - 1] == -1 && map[1][x][y - 1] != turn && !kingdoms[map[1][x][y - 1]].dead) {
            opponent = map[1][x][y - 1];
            dijkstraX = x;
            dijkstraY = y - 1;
            return 3;
        }
        if (map[0][x][y - 1] == -2 && villages[map[1][x][y - 1]].kingdom != 0 &&
            villages[map[1][x][y - 1]].kingdom != turn) {
            opponent = villages[map[1][x][y - 1]].kingdom;
            dijkstraX = x;
            dijkstraY = y - 1;
            return 2;
        }
        if (map[0][x][y - 1] > 0 && map[1][x][y - 1] != 0 && map[1][x][y - 1] != turn) {
            opponent = map[1][x][y - 1];
            dijkstraX = x;
            dijkstraY = y - 1;
            return 1;
        }
    }

    if (y != (mapHeight-1)) {

        if (map[0][x][y + 1] == -1 && map[1][x][y + 1] != turn && !kingdoms[map[1][x][y + 1]].dead) {
            opponent = map[1][x][y + 1];
            dijkstraX = x;
            dijkstraY = y + 1;
            return 3;
        }
        if (map[0][x][y + 1] == -2 && villages[map[1][x][y + 1]].kingdom != 0 &&
            villages[map[1][x][y + 1]].kingdom != turn) {
            opponent = villages[map[1][x][y + 1]].kingdom;
            dijkstraX = x;
            dijkstraY = y + 1;
            return 2;
        }
        if (map[0][x][y + 1] > 0 && map[1][x][y + 1] != 0 && map[1][x][y + 1] != turn) {
            opponent = map[1][x][y + 1];
            dijkstraX = x;
            dijkstraY = y + 1;
            return 1;
        }
    }

    if (x != (mapWidth-1) ) {

        if (map[0][x + 1][y] == -1 && map[1][x + 1][y] != turn && !kingdoms[map[1][x + 1][y]].dead) {
            opponent = map[1][x + 1][y];
            dijkstraX = x + 1;
            dijkstraY = y;
            return 3;
        }
        if (map[0][x + 1][y] == -2 && villages[map[1][x + 1][y]].kingdom != 0 &&
            villages[map[1][x + 1][y]].kingdom != turn) {
            opponent = villages[map[1][x + 1][y]].kingdom;
            dijkstraX = x + 1;
            dijkstraY = y;
            return 2;
        }
        if (map[0][x + 1][y] > 0 && map[1][x + 1][y] != 0 && map[1][x + 1][y] != turn) {
            opponent = map[1][x + 1][y];
            dijkstraX = x + 1;
            dijkstraY = y;
            return 1;
        }
    }
    return 0;
}

void DeleteKingdom(int id) {
    kingdoms[id].food = 0;
    kingdoms[id].foodX = 0;
    kingdoms[id].gold = 0;
    kingdoms[id].goldX = 0;
    kingdoms[id].worker = 0;
    kingdoms[id].soldier = 0;
    for (int i = 0; i < kingdoms[id].roadNumber; ++i) {
        map[1][(int)kingdoms[id].road[i].x][(int)kingdoms[id].road[i].y] = 0;
    }
    kingdoms[id].roadNumber=0;
    for (int i = 0; i < villageNumber; ++i) {
        if (villages[i].kingdom  == id)
            villages[i].kingdom = 0;
    }
    kingdoms[id].villageNumber = 0;
    kingdoms[id].color = BLACK;
    kingdoms[id].dead=1;

    int aliveCount=0, alive=0;
    for (int i = 1; i <= kingdomNumber; ++i) {
        aliveCount += !kingdoms[i].dead;
        alive += i * !kingdoms[i].dead;
    }
    if (aliveCount == 1) {
        turn++;
        winner = alive;
        mode = 3;
    }
    else {
        turn++;
        mode = 0;
    }
}

