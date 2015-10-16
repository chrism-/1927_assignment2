// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h"

struct dracView {
    GameView gameView;

    Round roundCounter;
    int whosTurn;
    int gamePoints;
    int location[NUM_PLAYERS];
    int blood[NUM_PLAYERS];
    LocationID trail[NUM_PLAYERS][TRAIL_SIZE];

    char whatsThere[NUM_MAP_LOCATIONS];
};


// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    DracView dracView = malloc(sizeof(struct dracView));

    // inits
    dracView->gameView = newGameView(pastPlays,messages);

    dracView->roundCounter = getRound(dracView->gameView);
    dracView->whosTurn = getCurrentPlayer(dracView->gameView);
    dracView->gamePoints = getScore(dracView->gameView);

    dracView->location[PLAYER_LORD_GODALMING]=getLocation(dracView->gameView,PLAYER_LORD_GODALMING);
    dracView->location[PLAYER_DR_SEWARD]=getLocation(dracView->gameView,PLAYER_DR_SEWARD);
    dracView->location[PLAYER_VAN_HELSING]=getLocation(dracView->gameView,PLAYER_VAN_HELSING);
    dracView->location[PLAYER_MINA_HARKER]=getLocation(dracView->gameView,PLAYER_MINA_HARKER);
    dracView->location[PLAYER_DRACULA]=getLocation(dracView->gameView,PLAYER_DRACULA);


    dracView->blood[PLAYER_LORD_GODALMING]=getHealth(dracView->gameView,PLAYER_LORD_GODALMING);
    dracView->blood[PLAYER_DR_SEWARD]=getHealth(dracView->gameView,PLAYER_DR_SEWARD);
    dracView->blood[PLAYER_VAN_HELSING]=getHealth(dracView->gameView,PLAYER_VAN_HELSING);
    dracView->blood[PLAYER_MINA_HARKER]=getHealth(dracView->gameView,PLAYER_MINA_HARKER);
    dracView->blood[PLAYER_DRACULA]=getHealth(dracView->gameView,PLAYER_DRACULA);

    getHistory(dracView->gameView,PLAYER_LORD_GODALMING,dracView->trail[PLAYER_LORD_GODALMING]);
    getHistory(dracView->gameView,PLAYER_DR_SEWARD,dracView->trail[PLAYER_DR_SEWARD]);
    getHistory(dracView->gameView,PLAYER_VAN_HELSING,dracView->trail[PLAYER_VAN_HELSING]);
    getHistory(dracView->gameView,PLAYER_MINA_HARKER,dracView->trail[PLAYER_MINA_HARKER]);
    getHistory(dracView->gameView,PLAYER_DRACULA,dracView->trail[PLAYER_DRACULA]);

    int numberOfChars = 0;
    while(pastPlays[numberOfChars] != 0){
        numberOfChars++;
    }
    int i;
    char abbrev[3];
    abbrev[2]=0;
    LocationID ID = 0;

    for(i = 33; i<numberOfChars; i+=40){
        abbrev[0] = pastPlays[i];
        abbrev[1] = pastPlays[i+1];
        ID = abbrevToID(abbrev);


        if(pastPlays[i+2] == 'T')
            dracView->whatsThere[ID] = 'T';
        if(pastPlays[i+3] == 'V')
            dracView->whatsThere[ID] = 'V';

    }

    return dracView;
}


// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    disposeGameView(toBeDeleted->gameView);
    free(toBeDeleted->gameView);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    return currentView->roundCounter;
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    return currentView->gamePoints;
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    return currentView->blood[player];
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    return currentView->location[player];
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
    *start = currentView->trail[player][1];
    *end = currentView->trail[player][0];
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    *numTraps = 0;
    *numVamps = 0;
    if (currentView->whatsThere[where] == 'T') {
        *numTraps = 1;
    }
    if (currentView->whatsThere[where] == 'V') {
        *numVamps = 1;
    }
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    int a = 0;
    while(a < TRAIL_SIZE){
        trail[a] = currentView->trail[player][a];
        a++;
    }
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    return whereCanTheyGo(currentView, numLocations, getCurrentPlayer(currentView->gameView), road, FALSE, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    int i, numValidLocations, index;
    LocationID forbidden;
    LocationID *validLocations;

    LocationID *locations = connectedLocations(currentView->gameView,
                               numLocations,
                               getLocation(currentView->gameView, player),
                               player,
                               getRound(currentView->gameView),
                               road, rail, sea);
    if(player == PLAYER_DRACULA){
        forbidden = ST_JOSEPH_AND_ST_MARYS;
    }

    numValidLocations = 0;
    for(i = 0; i < (*numLocations); i++){
        if(locations[i] != forbidden){
            numValidLocations++;
        }
    }

    index = 0;
    validLocations = malloc(sizeof(LocationID) * numValidLocations);
    for(i = 0; i < numValidLocations; i++){
        if(locations[i] != forbidden){
            validLocations[index] = locations[i];
            index++;
        }
    }

    free(locations);
    *numLocations = numValidLocations;
    return validLocations;
}
