// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Game.h"
#include "Map.h"
#include "GameView.h"
#include "HunterView.h"
#include "hunter.h"


// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015
#define strEQ(s,t) (strcmp((s),(t)) == 0)
#define strLT(s,t) (strcmp((s),(t)) < 0)

static Link newNode(char *);
static void disposeNode(Link);
static int  findNode(Link,char *,Link *,Link *);

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

//LocationID getNextLocationInPath(char* orig, char* dest) {
//	LocationID nextLocation = malloc();
//   = whereCanIgo(gameState, &numLocations, TRUE, TRUE, TRUE);
//}

// newSet()
// - create an initially empty Set
Set newSet()
{
	Set new = malloc(sizeof(SetRep));
	assert(new != NULL);
	new->nelems = 0;
	new->elems = NULL;
	return new;
}

// disposeSet(Set)
// - clean up memory associated with Set
void disposeSet(Set s)
{
	if (s == NULL) return;
	Link next, curr = s->elems;
	while (curr != NULL) {
		next = curr->next;
		disposeNode(curr);
		curr = next;
	}
}

// insertInto(Set,Str)
// - ensure that Str is in Set
void insertInto(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (found) return; // already in Set
	Link new = newNode(str);
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	}
	else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
}

// dropFrom(Set,Str)
// - ensure that Str is not in Set
void dropFrom(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (!found) return;
	s->nelems--;
	if (prev == NULL)
		s->elems = curr->next;
	else
		prev->next = curr->next;
	disposeNode(curr);
}

// isElem(Set,Str)
// - check whether Str is contained in Set
int isElem(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	return findNode(s->elems,str,&curr,&prev);
}

// nElems(Set)
// - return # elements in Set
int  nElems(Set s)
{
	assert(s != NULL);
	return s->nelems;
}

// Helper functions

static Link newNode(char *str)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = strdup(str);
	new->next = NULL;
	return new;
}

static void disposeNode(Link curr)
{
	assert(curr != NULL);
	free(curr->val);
	free(curr);
}

// findNode(L,Str)
// - finds where Str could be added into L
// - if already in L, curr->val == Str
// - if not already in L, curr and prev indicate where to insert
// - return value indicates whether Str found or not
static int findNode(Link list, char *str, Link *cur, Link *pre)
{
	Link curr = list, prev = NULL;
	while (curr != NULL && strLT(curr->val,str)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr; *pre = prev;
	return (curr != NULL && strEQ(str,curr->val));
}

// Queue.h ... implementation of Queue ADT
// assumes that Item is an assignable type
// (e.g. int, pointer) defined in Queue.h

// create new empty Queue
Queue newQueue()
{
	Queue q;
	q = malloc(sizeof(QueueRep));
	assert(q != NULL);
	q->head = NULL;
	q->tail = NULL;
	return q;
}

// free memory used by Queue
void dropQueue(Queue Q)
{
	QueueNode *curr, *next;
	assert(Q != NULL);
	// free list nodes
	curr = Q->head;
	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	// free queue rep
	free(Q);
}

// add item at end of Queue
void QueueJoin(Queue Q, LocationID* it)
{
	assert(Q != NULL);
	QueueNode *new = malloc(sizeof(QueueNode));
	assert(new != NULL);
	new->value = it;
	new->next = NULL;
	if (Q->head == NULL)
		Q->head = new;
	if (Q->tail != NULL)
		Q->tail->next = new;
	Q->tail = new;
}

// remove item from front of Queue
LocationID* QueueLeave(Queue Q)
{
	assert(Q != NULL);
	assert(Q->head != NULL);
	LocationID* it = Q->head->value;
	QueueNode *old = Q->head;
	Q->head = old->next;
	if (Q->head == NULL) Q->tail = NULL;
	free(old);
	return it;
}

// check for no items
int QueueIsEmpty(Queue Q)
{
	return (Q->head == NULL);
}
