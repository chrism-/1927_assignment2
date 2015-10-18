// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hunter.h"
#include "Game.h"
#include "HunterView.h"

// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015
#define strEQ(s,t) (strcmp((s),(t)) == 0)
#define strLT(s,t) (strcmp((s),(t)) < 0)

typedef struct Node *Link;

typedef struct Node {
	char *val;
	Link  next;
} Node;

typedef struct SetRep {
	int   nelems;
	Link  elems;
} SetRep;

// queue.c by John Shepherd

typedef struct QueueNode {
	LocationID* value;
	struct QueueNode *next;
} QueueNode;

typedef struct QueueRep {
	QueueNode *head;  // ptr to first node
	QueueNode *tail;  // ptr to last node
} QueueRep;


// TODO: function: find fastest way between 2 points on the map
// TODO: function: write data into string (encode data)

struct message {
    PlayerID id;
    int turnNumber;
    int flagType; // trailfound, follow
    LocationID locationID;
};

void decideHunterMove(HunterView gameState)
{
    PlayerID id = whoAmI(gameState);

    // start of game
    if (giveMeTheRound(gameState) == 0) {
		// seed the random function
		// may not need because we don't know the size of the possible destinations...

		// make one of them spawn near castle dracula
        if (id == PLAYER_LORD_GODALMING) {
            registerBestPlay("CD","I'm on holiday in Castle Drac");
        } else if (id == PLAYER_DR_SEWARD) {
            registerBestPlay("LO","I'm on holiday in London");
        } else if (id == PLAYER_VAN_HELSING) {
            registerBestPlay("LO","I'm on holiday in London");
        } else if (id == PLAYER_MINA_HARKER) {
            registerBestPlay("LO","I'm on holiday in London");
        }
    } else {
		int numLocations = 0;
		LocationID* places = whereCanIgo(gameState, &numLocations, TRUE, TRUE, TRUE);
		// random moves (take the first 1 atm)

		registerBestPlay(idToName(places[0]), "On holiday");
		printf("pos: %s\n", idToName(places[0]));
	}

    // get past messages
    // if flagged trail found
        // if trail not found atm
            // find path to flagged position
    // else
        // flag trail found
        // flag position
        // find path to flagged position

    // get trail
    // go directly to first position of the trail, alert other hunters
        // store position of first position of the trail
        // flag position found to other hunters


	//whereCanIgo(gameState);
	//if (id == PLAYER_LORD_GODALMING) {
    //    registerBestPlay("GE","I'm on holiday in Geneva");
    //} else {
	//	registerBestPlay("LO","I'm on holiday in London");
	//}
}
