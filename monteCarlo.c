#include "monteCarlo.h"
#include "Map.h"
#include "War.h"
#include "raylib.h"
#include "stdlib.h"
#include <math.h>

#define constant 2

node* root;
int totalVisits;

typedef struct node node;

void monte() {
    node *current;
    current = (node*) malloc(sizeof(node));
    current->parent = NULL;
    current->children = NULL;
    SaveGame(current->state);
    current->visits=current->winCount=0;
    root = current;

    for (int i = 0; i < 10; ++i) {
        expand(current);
        for (int j=0; j<current->childCount; j++) {
            simulation(current->children[j]->state);
        }
        backpropagation();
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

    while (!winner) {

    }
    return (winner==root->state->turn);
}

void backpropagation() {

}

int possibleMoves (gameState* state, Vector2 map0) {
    state->kingdom[state->turn].availableNumber = 0;
    int check = checkNeighbors(state->kingdom[state->turn].x, state->kingdom[state->turn].y, map0);

    for (int i = 0; i <state->kingdom[state->turn].roadNumber && check==0; ++i) {
        check = checkNeighbors(state->kingdom[state->turn].road[i].x, state->kingdom[state->turn].road[i].y, map0);
    }

    for(int i=0; i<state->villageNumber && check==0; i++) {
        if(state->villages[i].kingdom == state->turn) {
            check = checkNeighbors(state->villages[i].x, state->villages[i].y, map0);
        }
    }
    int moveCount = state->kingdom[state->turn].roadNumber;
    if (state->kingdom[state->turn].foodX < 3 && state->kingdom[state->turn].worker < 4)
        moveCount++;
    if (state->kingdom[state->turn].worker <4)
        moveCount++;
    moveCount++;
    return moveCount;
}