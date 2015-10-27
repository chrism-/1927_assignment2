// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"

void decideHunterMove(HunterView gameState)
{
    char* nextPos = NULL;

   	if (giveMeTheRound(gameState) == 0){
      nextPos = "GE";
  	} else {
        int numLocations;
        LocationID* possibleDest = whereCanIgo(gameState, &numLocations, 1, 0, 1);

        srand((unsigned int)time(NULL)); //seed random

        nextPos = idToName(possibleDest[rand()%numLocations]);
	}

	registerBestPlay(nextPos, "hello world");
}
