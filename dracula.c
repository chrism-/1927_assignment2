// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "DracView.h"

void decideDraculaMove(DracView gameState)
{
	printf("Monday 4-6 group.\n");
	// TODO ...
	// Replace the line below by something better
	
	/* 
	// improved AI in the future
	int hunterPosition[4];
	char* nextPos = NULL;
	int Round;
	int *numLocations = 0;

	hunterPosition[PLAYER_LORD_GODALMING] = whereIs(gameState, PLAYER_LORD_GODALMING);
	hunterPosition[PLAYER_DR_SEWARD] = whereIs(gameState, PLAYER_DR_SEWARD);
	hunterPosition[PLAYER_VAN_HELSING] = whereIs(gameState, PLAYER_VAN_HELSING);
	hunterPosition[PLAYER_MINA_HARKER] = whereIs(gameState, PLAYER_MINA_HARKER);

	Round = giveMeTheRound(gameState);
	if (Round == 0){
		nextPos = "PA";
	}

	// where can i go?

	int DraculaNextPossibleMove[71] = {0};
	DraculaNextPossibleMove = whereCanIgo(gameState, numLocations, 1, 1);

	*/
   char* nextPos = NULL;
	char* prevPos = NULL;
	char* HidePos = NULL;
   if (giveMeTheRound(gameState) == 0){
      nextPos = "MR";
   } else if (strcmp(nextPos,"MR") == 0) { 
      nextPos = "TO";
      prevPos = "MR";
   } else if (strcmp(nextPos,"TO") == 0) { 
      nextPos = "BA";
      prevPos = "TO";
   } else if (strcmp(nextPos,"BA") == 0) { 
      nextPos = "SR";
      prevPos = "BA";
   } else if (strcmp(nextPos,"SR") == 0 && strcmp(prevPos,"BA") == 0) { 
      nextPos = "MA";
      prevPos = "SR";
   } else if (strcmp(nextPos,"MA") == 0) { 
      nextPos = "HI";
      HidePos = "MA";
   } else if (strcmp(nextPos,"HI") == 0 && strcmp(HidePos,"MA") == 0){
      nextPos = "AL";
      prevPos = "HI";
   } else if (strcmp(nextPos,"AL") == 0) { 
      nextPos = "GR";
      prevPos = "AL";
   } else if (strcmp(nextPos,"GR") == 0) { 
      nextPos = "LS";
      prevPos = "GR";
   } else if (strcmp(nextPos,"LS") == 0) { 
      nextPos = "SN";
      prevPos = "LS";
   } else if (strcmp(nextPos,"SN") == 0) { 
      nextPos = "SR";
      prevPos = "SN";
   } else if (strcmp(nextPos,"SR") == 0 && strcmp(prevPos,"SN") == 0) { 
      nextPos = "BO";
      prevPos = "SR";
   } else if (strcmp(nextPos,"BO") == 0) { 
      nextPos = "CF";
      HidePos = "BO";
   } else if (strcmp(nextPos,"CF") == 0) { 
      nextPos = "GE";
      HidePos = "CF";
   } else if (strcmp(nextPos,"GE") == 0) { 
      nextPos = "ZU";
      HidePos = "GE";
   } else if (strcmp(nextPos,"ZU") == 0) { 
      nextPos = "MI";
      HidePos = "ZU";
   } else if (strcmp(nextPos,"MI") == 0) { 
      nextPos = "HI";
      HidePos = "MI";
   } else if (strcmp(nextPos,"HI") == 0 && strcmp(prevPos,"MI") == 0){
   	nextPos = "MR";
   	prevPos = "HI";
   }

	registerBestPlay(nextPos,"Mwuwuwuhahahaha");
}
