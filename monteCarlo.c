#include "monteCarlo.h"
#include "Map.h"
#include "War.h"
#include "raylib.h"
#include "stdlib.h"
#include <math.h>

#define constant 2
#define iterations 10

node* root;
int totalVisits;

typedef struct node node;

void monte() {
    monteCarlo = 1;
    node *current;
    current = (node*) malloc(sizeof(node));
    current->parent = NULL;
    current->children = NULL;
    SaveGame(current->state);
    current->visits=current->winCount=0;
    root = current;

    for (int i = 0; i < iterations ; ++i) {
        expand(current);
        for (int j = 0; j < current->childCount; ++j) {
            int gameResult = simulation(current->children[j]->state);
            backpropagation(current->children[j],gameResult);
        }
        current = selection();
    }
}

node* selection() {
    node* best = NULL;
    double maxUCB;
    node* parent = root;
    while (parent->children) {
        maxUCB = -1;
        for (int i = 0; i < parent->childCount; ++i) {
            double ucb = ((parent->children[i]->winCount/parent->children[i]->visits) +
                       constant* sqrt(log(totalVisits)/parent->children[i]->visits));
            if (ucb > maxUCB) {
                maxUCB = ucb;
                best = parent->children[i];
            }
        }
         parent = best;
    }
    return best;
}

void expand(node* parent) {

}

int simulation(gameState* state) {
    gameState mainGame, changer=*state;
    SaveGame(&mainGame);
    while (!winner) {
            if (turn > kingdomNumber) {
                turn = 1;
                for (int i = 1; i <= kingdomNumber; i++) {
                    if (kingdoms[i].dead)
                        continue;
                    kingdoms[i].food += kingdoms[i].foodX;
                    kingdoms[i].gold += kingdoms[i].goldX;
                }
            }
        int move = GetRandomValue(1, possibleMoves(&changer)+4);
        if (move <= changer.kingdom[changer.turn].availableNumber) {
            roadX = changer.kingdom[changer.turn].available[move-1].x;
            roadY = changer.kingdom[changer.turn].available[move-1].y;
            RoadMaker();
        }
        else {
            if (move - changer.kingdom[changer.turn].availableNumber ==1) { //add food
                if (state->kingdom[state->turn].foodX < 3 && state->kingdom[state->turn].worker < 4) {
                    action = 2;
                    mode1();
                }
                else move++;
            }
            if (move - changer.kingdom[changer.turn].availableNumber == 2) { //add worker
                if (state->kingdom[state->turn].worker <4 && state->kingdom[state->turn].food >=3) {
                    action = 2;
                    mode1();
                }
                else move++;
            }
            else if (changer.kingdom[changer.turn].gold >=2) { //add soldier
                action = 3;
                mode1();
            }
            else //do nothing
                changer.turn++;
        }
    }
    LoadGame(&mainGame);
    return (winner == root->state->turn);
}

void backpropagation(node* node, int result) {
    while (node != NULL) {
        node->visits++;
        node->winCount += result;
        node = node->parent;
    }
}

int possibleMoves (gameState* state) {
    state->kingdom[state->turn].availableNumber = 0;
    int check = checkNeighbors(state->kingdom[state->turn].x, state->kingdom[state->turn].y);

    for (int i = 0; i <state->kingdom[state->turn].roadNumber && check==0; ++i) {
        check = checkNeighbors(state->kingdom[state->turn].road[i].x, state->kingdom[state->turn].road[i].y);
    }

    for(int i=0; i<state->villageNumber && check==0; i++) {
        if(state->villages[i].kingdom == state->turn) {
            check = checkNeighbors(state->villages[i].x, state->villages[i].y);
        }
    }
    int moveCount = state->kingdom[state->turn].roadNumber;
    if (state->kingdom[state->turn].foodX < 3 && state->kingdom[state->turn].worker < 4)
        moveCount++;
    if (state->kingdom[state->turn].worker <4 && state->kingdom[state->turn].food >=3)
        moveCount++;
    moveCount+=2;
    return moveCount;
}