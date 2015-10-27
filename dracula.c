// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "Places.h"
#include "DracView.h"
#include "dracula.h"

void decideDraculaMove(DracView gameState)
{	
    char* nextPos = NULL;

   	if (giveMeTheRound(gameState) == 0){
      nextPos = "MR";
  	} else {
        int numLocations;
        LocationID* possibleDest = whereCanIgo(gameState, &numLocations, 1, 0);

        srand((unsigned int)time(NULL)); //seed random

        nextPos = idToName(possibleDest[rand()%numLocations]);
	}

	PlayerMessage msg = "Dracula MSG";

	registerBestPlay(nextPos,msg);
}

