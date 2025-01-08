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
//        for (int u: children[current]) {
//            simulation(child[i]);
//        }
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

int possibleMoves () {

}