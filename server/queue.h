#include "libs.h"

#define TRUE  1
#define FALSE	0

/* a link in the queue, holds the info and point to the next Node*/

typedef struct Node_t {
	int data;
	struct Node_t *prev;
} NODE;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue {
	NODE *head;
	NODE *tail;
	int size;
	int limit;
} Queue;

void ConstructQueue(Queue **queue);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, NODE *item);
int Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);

