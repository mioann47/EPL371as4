#include "queue.h"

void ConstructQueue(Queue** queue) {
	*queue = (Queue*) malloc(sizeof(Queue));
	if (*queue == NULL) {
		return;
	}

	(*queue)->limit = 65535;
	(*queue)->size = 0;
	(*queue)->head = NULL;
	(*queue)->tail = NULL;

}

void DestructQueue(Queue *queue) {
	// NODE *pN;

	while (!isEmpty(queue)) {
		Dequeue(queue);
		// free(pN);
	}
	free(queue);
}

int Enqueue(Queue *pQueue, NODE *item) {
	/* Bad parameter */
	if ((pQueue == NULL) || (item == NULL)) {
		return FALSE;
	}
	// if(pQueue->limit != 0)
	if (pQueue->size >= pQueue->limit) {
		return FALSE;
	}
	/*the queue is empty*/
	item->prev = NULL;
	if (pQueue->size == 0) {
		pQueue->head = item;
		pQueue->tail = item;

	} else {
		/*adding item to the end of the queue*/
		pQueue->tail->prev = item;
		pQueue->tail = item;
	}
	pQueue->size++;
	return TRUE;
}

int Dequeue(Queue *pQueue) {
	/*the queue is empty or bad param*/
	NODE *item;
	if (isEmpty(pQueue))
		return -1;
	item = pQueue->head;
	pQueue->head = (pQueue->head)->prev;
	pQueue->size--;
	int temp = item->data;
	free(item);
	return temp;
}

int isEmpty(Queue* pQueue) {
	if (pQueue == NULL) {
		return -1;
	}
	if (pQueue->size == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*
 int main() {
 int i;
 Queue *pQ = ConstructQueue(7);
 NODE *pN;
 int x;
 for (i = 0; i < 9; i++) {
 pN = (NODE*) malloc(sizeof (NODE));
 pN->data = 100 + i;
 Enqueue(pQ, pN);
 }

 while (!isEmpty(pQ)) {
 x = Dequeue(pQ);
 printf("\nDequeued: %d", x);

 }
 DestructQueue(pQ);
 return (EXIT_SUCCESS);
 }*/
