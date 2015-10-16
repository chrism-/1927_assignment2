// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>

#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"

struct hunterView {
    GameView gameView;

    Round roundCounter;
    int whosTurn;
    int gamePoints;
    int location[NUM_PLAYERS];
    int blood[NUM_PLAYERS];
    LocationID trail[NUM_PLAYERS][TRAIL_SIZE];
};


// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    HunterView hunterView = malloc(sizeof(struct hunterView));

    // inits
    hunterView->gameView = newGameView(pastPlays,messages);

    // view stuff
    hunterView->roundCounter = getRound(hunterView->gameView);
    hunterView->whosTurn = getCurrentPlayer(hunterView->gameView);
    hunterView->gamePoints = getScore(hunterView->gameView);

    hunterView->location[PLAYER_LORD_GODALMING]=getLocation(hunterView->gameView,PLAYER_LORD_GODALMING);
    hunterView->location[PLAYER_DR_SEWARD]=getLocation(hunterView->gameView,PLAYER_DR_SEWARD);
    hunterView->location[PLAYER_VAN_HELSING]=getLocation(hunterView->gameView,PLAYER_VAN_HELSING);
    hunterView->location[PLAYER_MINA_HARKER]=getLocation(hunterView->gameView,PLAYER_MINA_HARKER);
    hunterView->location[PLAYER_DRACULA]=getLocation(hunterView->gameView,PLAYER_DRACULA);

    hunterView->blood[PLAYER_LORD_GODALMING]=getHealth(hunterView->gameView,PLAYER_LORD_GODALMING);
    hunterView->blood[PLAYER_DR_SEWARD]=getHealth(hunterView->gameView,PLAYER_DR_SEWARD);
    hunterView->blood[PLAYER_VAN_HELSING]=getHealth(hunterView->gameView,PLAYER_VAN_HELSING);
    hunterView->blood[PLAYER_MINA_HARKER]=getHealth(hunterView->gameView,PLAYER_MINA_HARKER);
    hunterView->blood[PLAYER_DRACULA]=getHealth(hunterView->gameView,PLAYER_DRACULA);

    getHistory(hunterView->gameView,PLAYER_LORD_GODALMING,hunterView->trail[PLAYER_LORD_GODALMING]);
    getHistory(hunterView->gameView,PLAYER_DR_SEWARD,hunterView->trail[PLAYER_DR_SEWARD]);
    getHistory(hunterView->gameView,PLAYER_VAN_HELSING,hunterView->trail[PLAYER_VAN_HELSING]);
    getHistory(hunterView->gameView,PLAYER_MINA_HARKER,hunterView->trail[PLAYER_MINA_HARKER]);
    getHistory(hunterView->gameView,PLAYER_DRACULA,hunterView->trail[PLAYER_DRACULA]);

    return hunterView;
}


// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    disposeGameView(toBeDeleted->gameView);
    free(toBeDeleted->gameView);
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    return currentView->roundCounter;
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    return currentView->whosTurn;
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return currentView->gamePoints;
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    return currentView->blood[player];
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    return currentView->location[player];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player, LocationID trail[TRAIL_SIZE])
{
    int a = 0;
    while(a < TRAIL_SIZE){
        trail[a] = currentView->trail[player][a];
        a++;
    }
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations, int road,int rail, int sea)
{
    // COMP1927 15s2 ... basic HunterView (supplied code)
    // Code by TheGroup from COMP1927 14s2
    return whereCanTheyGo(currentView, numLocations, getCurrentPlayer(currentView->gameView), road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations, PlayerID player, int road, int rail, int sea)
{
    // COMP1927 15s2 ... basic HunterView (supplied code)
    // Code by TheGroup from COMP1927 14s2
    // Modified by Chris Miles 2015
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
