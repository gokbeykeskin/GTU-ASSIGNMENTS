#include "../headers/queue.h"
#include <stdlib.h>
#include <stdio.h>

//create queue
queue_t *queue_create() {
	queue_t *queue = malloc(sizeof(queue_t));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

//destroy queue
void queue_destroy(queue_t *queue) {
	while(!queue_empty(queue)) {
		dequeue(queue);
	}
	free(queue);
}

//add an element to the queue
void enqueue(queue_t *queue, int* data) {
	node_t *node = malloc(sizeof(node_t));
	node->data = data;
	node->next = NULL;
	if(queue_empty(queue)) {
		queue->head = node;
	} else {
		queue->tail->next = node;
	}
	queue->tail = node;
}

//remove an element from the queue
int* dequeue(queue_t *queue) {
    if(queue_empty(queue)) {
        return NULL;
    }
	int *data = queue->head->data;
	node_t *node = queue->head;
	queue->head = queue->head->next;
	free(node);
	if(queue_empty(queue)) {
		queue->tail = NULL;
	}
	return data;
}

//check if queue is empty
int queue_empty(queue_t *queue) {
	return queue->head == NULL;
}