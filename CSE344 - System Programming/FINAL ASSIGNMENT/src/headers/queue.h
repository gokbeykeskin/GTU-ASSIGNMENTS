#ifndef MQUEUE_H
#define MQUEUE_H

typedef struct node {
	int *data;
	struct node *next;
} node_t;

typedef struct queue {
	node_t *head;
	node_t *tail;
} queue_t;

//create queue
queue_t *queue_create();

//destroy queue
void queue_destroy(queue_t *queue);

//add an element to the queue
void enqueue(queue_t *queue, int* data);

//remove an element from the queue
int* dequeue(queue_t *queue);

//check if queue is empty
int queue_empty(queue_t *queue);

#endif