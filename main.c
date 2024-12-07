#include <stdio.h>
#include "raylib.h"
#include "InitialMap.h"

// Program main entry point
int main() {

    initialMapMaker();
    makeKingdom();
    makeVillage();
    makeBarrier();
    mapDrawer();
    return 0;
}
