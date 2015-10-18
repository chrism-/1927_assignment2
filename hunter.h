// hunter.h
// Interface to your "Fury of Dracula" hunter AI
// By: TeamDracula
// Date: 1 January 2012
// Version: 1.0

// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#include "Game.h"
#include "HunterView.h"

typedef struct Node *Link;
typedef struct SetRep *Set;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,char *);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);
void showSet(Set);

Link newNode(char *);
void disposeNode(Link);
int  findNode(Link,char *,Link *,Link *);

// Queue.h ... interface to Queue-of-BSTree-nodes ADT
typedef struct QueueRep *Queue;

Queue newQueue(); // create new empty queue
void dropQueue(Queue); // free memory used by queue
void showQueue(Queue); // display as 3 > 5 > 4 > ...
void QueueJoin(Queue,LocationID*); // add item on queue
LocationID* QueueLeave(Queue); // remove item from queue
int QueueIsEmpty(Queue); // check for no items



typedef struct message Message;

void decideHunterMove(HunterView gameState);
