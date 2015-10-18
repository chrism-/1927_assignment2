// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"

struct gameView {
    Map map;
    Round roundCounter;
    int whoseTurn;
    int gamePoints;
    int location[NUM_PLAYERS];
    int blood[NUM_PLAYERS];
    int trail[NUM_PLAYERS][TRAIL_SIZE];
    PlayerMessage** messages;
};


// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = malloc(sizeof(struct gameView));

    gameView->messages = &messages;

    gameView->map = newMap();

    //Extract information from the pastPlays
    //how many characters are there in pastPlays
    int numberOfChars = 0;
    while(pastPlays[numberOfChars] != 0){
        numberOfChars++;
    }

    //Initialise the roundCounter
    if(numberOfChars == 0){
        gameView->roundCounter = 0;
    }else gameView->roundCounter = ((int)numberOfChars+1)/40;


    //Initialise the turn
    //whoseTurn has to be passed to getCurrentPlayer()
    gameView->whoseTurn = (numberOfChars+1)/8;

    //Initialise the game points
    gameView->gamePoints = GAME_START_SCORE;
    //gamepoints decreases by one every time dracula's turn has been finished
    gameView->gamePoints -= (int)(numberOfChars+1)/ 40;
    //gamepoints decreases by 13 when vampire is mature(six round after dracula place the immature vampire)
    int dracVampire = 36;
    while(dracVampire < (numberOfChars+1)){
        int vampireDead = 0;
        int a = 0;
        for(a = dracVampire + 7; a < dracVampire + 40 * 6; a += 8){
            int b = dracVampire;
            while(pastPlays[b] != '.' && ((b-3)/8 % 5) != 4){
                if(pastPlays[b] == 'V') {
                    vampireDead = 1;
                }
                b++;
            }
            //six rounds
            if(vampireDead){
                gameView->gamePoints -= SCORE_LOSS_VAMPIRE_MATURES;
                break;
            }
        }
        //dracula can place immature vampire every 13 rounds
        dracVampire = dracVampire + 40 * 13;
    }

    //Initialise the blood and life points
    int a = 0;
    int b = 0;

    gameView->blood[PLAYER_LORD_GODALMING] = GAME_START_HUNTER_LIFE_POINTS;
    gameView->blood[PLAYER_DR_SEWARD] = GAME_START_HUNTER_LIFE_POINTS;
    gameView->blood[PLAYER_VAN_HELSING] = GAME_START_HUNTER_LIFE_POINTS;
    gameView->blood[PLAYER_MINA_HARKER] = GAME_START_HUNTER_LIFE_POINTS;
    gameView->blood[PLAYER_DRACULA] = GAME_START_BLOOD_POINTS;

    //change of hunters' health
    for(a = 3; a < numberOfChars; a += 8){
        b = a;
        while(pastPlays[b] != '.' && ((a-3)/8 % 5) != 4){
            if(pastPlays[b] == 'T'){
                gameView->blood[(a-3)/8 % 5] -= LIFE_LOSS_TRAP_ENCOUNTER;
                if(gameView->blood[(a-3)/8 % 5] < 0) gameView->blood[(a-3)/8 % 5] = GAME_START_HUNTER_LIFE_POINTS;
            }
            if(pastPlays[b] == 'D') {
                gameView->blood[(a-3)/8 % 5] -= LIFE_LOSS_DRACULA_ENCOUNTER;
                gameView->blood[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
            }
            b++;
        }
    }
    //change of dracula's blood
    //dracula hate water
    for(a = 33; a < numberOfChars; a += 40){
        b = a;
        char message[3];
        message[0] = pastPlays[a];
        message[1] = pastPlays[a+1];
        message[2] = 0;
        if((message[0] == 'C' && message[1] == '?')
            || (message[0] == 'H' && message[1] == 'I')
            || (message[0] == 'T' && message[1] == 'P')){
            break;
        }else if(abbrevToID(message) >= MIN_MAP_LOCATION && abbrevToID(message) <= MAX_MAP_LOCATION){
            if(idToType(abbrevToID(message)) == SEA) {
                gameView->blood[PLAYER_DRACULA] -= 2;
            }
        }else if(message[0] == 'S' && message[1] == '?'){
            gameView->blood[PLAYER_DRACULA] -= 2;
        }else if(message[1] >= '1' && message[1] <= '5'){
            message[0] = pastPlays[b - 40 * (message[1] - 48)];
            message[1] = pastPlays[b - 40 * (message[1] - 48) + 1];
            if(message[0] == 'S' && message[1] == '?'){
                gameView->blood[PLAYER_DRACULA] -= 2;
                break;
            }else if(idToType(abbrevToID(message)) == SEA) gameView->blood[PLAYER_DRACULA] -= 2;
        }
    }


    //Initialise the location
    if(numberOfChars == 0){
        gameView->location[PLAYER_LORD_GODALMING] = UNKNOWN_LOCATION;
        gameView->location[PLAYER_DR_SEWARD] = UNKNOWN_LOCATION;
        gameView->location[PLAYER_VAN_HELSING] = UNKNOWN_LOCATION;
        gameView->location[PLAYER_MINA_HARKER] = UNKNOWN_LOCATION;
        gameView->location[PLAYER_DRACULA] = UNKNOWN_LOCATION;
    }else {
        int first = 0;
        if(numberOfChars < 32) {
            for(first = 1; first < numberOfChars; first += 8){
                char message[3];
                message[0] = pastPlays[first];
                message[1] = pastPlays[first+1];
                message[2] = 0;
                gameView->location[((first-1)/8) % 5] = abbrevToID(message);
            }
            int a = (39 - numberOfChars)/8;
            int b = 0;
            while (b < a){
                gameView->location[4-b] = UNKNOWN_LOCATION;
                b++;
            }
        } else{
            for(first = numberOfChars - 38;first < numberOfChars; first += 8){
                char message[3];
                message[0] = pastPlays[first];
                message[1] = pastPlays[first+1];
                message[2] = 0;

                if((first-1)/8 % 5 == 4){
                    if(message[1] == '?'){
                        switch(message[0]){
                            case 'C': gameView->location[PLAYER_DRACULA] = CITY_UNKNOWN; break;
                            case 'S': gameView->location[PLAYER_DRACULA] = SEA_UNKNOWN; break;
                        }
                    }else if(message[0] == 'H' && message[1] == 'I'){
                        gameView->location[PLAYER_DRACULA] = HIDE;
                    }else if(message[1] >= '1' && message[1] <= '5'){
                        gameView->location[PLAYER_DRACULA] = DOUBLE_BACK_1 + message[1] - 49;
                    }else if(message[0] == 'T' && message[1] == 'P'){
                        gameView->location[PLAYER_DRACULA] = TELEPORT;
                        gameView->blood[PLAYER_DRACULA] += LIFE_GAIN_CASTLE_DRACULA;
                    }
                    else{
                        gameView->location[PLAYER_DRACULA] = abbrevToID(message);
                        if(gameView->location[PLAYER_DRACULA] == CASTLE_DRACULA){
                            gameView->blood[PLAYER_DRACULA] += LIFE_GAIN_CASTLE_DRACULA;
                        }
                    }
                }else{
                    gameView->location[((first-1)/8) % 5] = abbrevToID(message);
                    if (gameView->location[((first-1)/8) % 5] == ST_JOSEPH_AND_ST_MARYS){
                        gameView->blood[((first-1)/8) % 5] = GAME_START_HUNTER_LIFE_POINTS;
                        gameView->gamePoints -= SCORE_LOSS_HUNTER_HOSPITAL;
                    }
                    if (gameView->roundCounter >= 2){
                        message[0] = pastPlays[first - 40];
                        message[1] = pastPlays[first - 39];
                        if(abbrevToID(message) == gameView->location[(first-1)/8 % 5]){
                            gameView->blood[(first-1)/8 % 5] += LIFE_GAIN_REST;
                            if(gameView->blood[(first-1)/8 % 5] > 9){
                                gameView->blood[(first-1)/8 % 5] = GAME_START_HUNTER_LIFE_POINTS;
                            }
                        }
                    }
                }
            }
        }
    }

    //initialising the trail
    a = 0;
    b = 0;
    int counter = 0;
    int totalNumberPlayers = 0;
    for (a = numberOfChars - 6; a > 0; a -= 8){
        int currentPlayer = 0;
        for(b = a ; b > 0; b -= 40){
            currentPlayer = (b-1)/8 % 5;
            char message[3];
            message[0] = pastPlays[b];
            message[1] = pastPlays[b+1];
            message[2] = 0;
            if((b-1)/8 % 5 == PLAYER_DRACULA){
                if(message[1] == '?'){
                    switch(message[0]){
                        case 'C': gameView->trail[PLAYER_DRACULA][counter] = CITY_UNKNOWN; break;
                        case 'S': gameView->trail[PLAYER_DRACULA][counter] = SEA_UNKNOWN; break;
                    }
                }else if (message[0] == 'H' && message[1] == 'I'){
                    gameView->trail[PLAYER_DRACULA][counter] = HIDE;
                }else if (message[1] >= '1' && message[1] <= '5'){
                    gameView->trail[PLAYER_DRACULA][counter] = DOUBLE_BACK_1 + message[1] - 49;
                }else if(message[0] == 'T' && message[1] == 'P'){
                    gameView->trail[PLAYER_DRACULA][counter] = TELEPORT;
                }else gameView->trail[PLAYER_DRACULA][counter] = abbrevToID(message);
            }else gameView->trail[currentPlayer][counter] = abbrevToID(message);
            counter++;
            if(counter == 6) break;
        }
        while(counter < 6){
            gameView->trail[currentPlayer][counter] = UNKNOWN_LOCATION;
            counter++;
        }
        counter = 0;
        totalNumberPlayers++;
        if(totalNumberPlayers > 4) break;
    }

    return gameView;
}


// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    disposeMap(toBeDeleted->map);
    free(toBeDeleted->map);
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    return currentView->roundCounter;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    assert(currentView != NULL);
    int n;
    n = currentView->whoseTurn % 5;
    switch(n){
        case 0: return PLAYER_LORD_GODALMING; break;
        case 1: return PLAYER_DR_SEWARD; break;
        case 2: return PLAYER_VAN_HELSING; break;
        case 3: return PLAYER_MINA_HARKER; break;
        case 4: return PLAYER_DRACULA; break;
        default: return 0;
    }
}

// Get the current score
int getScore(GameView currentView)
{
    assert(currentView->gamePoints != 0);
    return currentView->gamePoints;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    return currentView->blood[player];
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    return currentView->location[player];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player, LocationID trail[TRAIL_SIZE])
{
    int a = 0;
    while(a < TRAIL_SIZE){
        trail[a] = currentView->trail[player][a];
        a++;
    }
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    // COMP1927 15s2 ... basic HunterView (supplied code)
    // Code by TheGroup from COMP1927 14s2
    // Modified by Chris Miles 2015
    assert(validPlace(from));
    assert(player >= PLAYER_LORD_GODALMING && player <= PLAYER_DRACULA);

    int drac = (player == PLAYER_DRACULA);

    int railLength = (player + round) % 4;
    if (!rail) railLength = 0;
    if (drac) railLength = 0;


    LocationID  * res =  reachableLocations(currentView->map, numLocations, from, drac, railLength, road, sea);

    return res;
}

Map getMap(GameView currentView) {
    return currentView->map;
}

PlayerMessage* getMessageLastTurnCurPlayer(GameView currentView, PlayerID player) {
    int numOfItems = sizeof(*(currentView->messages))/sizeof(*(currentView->messages)[0]);
    //assert(numOfItems > 5);
    return currentView->messages[numOfItems-6];
}
