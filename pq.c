#include "pq.h"

#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct PriorityQueue {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    Node *n1 = node_create(0, 0);
    if (q) {
        q->top = 0;
        q->capacity = capacity;
        q->items = calloc(capacity, sizeof(Node *));
        for (uint32_t i = 0; i < q->capacity; ++i) {
            q->items[i] = n1;
        }
        return q;
    }
    return NULL;
}

void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->items) {
        for (uint32_t i = 0; i < (*q)->top; ++i) {
            node_delete(&(*q)->items[i]);
        }
        free((*q)->items);
        free(*q);
        *q = NULL;
    }
}

bool pq_empty(PriorityQueue *q) {
    if (q->top == 0) {
        return true;
    }
    return false;
}

bool pq_full(PriorityQueue *q) {
    if (q->top == q->capacity) {
        return true;
    }
    return false;
}

uint32_t pq_size(PriorityQueue *q) {
    return (q->top);
}

bool enqueue(PriorityQueue *q,
    Node *n) { // enqueue is important for approriately placing a node given its frequnecy
    if (q->top == q->capacity) { // if the queue is full we cannnot enqueue
        return false;
    }
    q->items[0] = n;
    q->top += 1;
    bool swapped = true;
    uint64_t total = q->capacity;
    while (
        swapped
        == true) { // Sorting algorithm to place the highest prioority/lowest frequecy nodes before the lowest priority/highest frequnecy nodes
        swapped = false;
        for (uint32_t i = 1; i < total; ++i) {
            if (q->items[i]->frequency < q->items[i - 1]->frequency) {
                //printf("items at i before \n");
                //node_print(q->items[i]);
                Node *temp_first = q->items[i];
                Node *temp_second = q->items[i - 1];
                q->items[i] = temp_second;
                q->items[i - 1] = temp_first;
                swapped = true;
                //printf("items at i after \n");
                //node_print(q->items[i]);
            }
        }
        total -= 1;
    }
    return true;
}

bool dequeue(PriorityQueue *q,
    Node **n) { // Since our queue is already sorted we place the highest priority node in n
    if (pq_empty(q)) {
        return false;
    }
    *n = q->items[(q->capacity - q->top)];
    Node *zNode = node_create(0, 0);
    //node_delete(&(q->items[(q->capacity - q->top)]));
    q->items[(q->capacity - q->top)] = zNode;
    q->top -= 1;

    return true;
}

void pq_print(PriorityQueue *
        q) { // Since we are always sorting our queue we wish to only print nodes that are relevant. Thus we begin at the capacity - top.
    for (uint64_t i = (q->capacity - q->top); i < (q->capacity); ++i) {
        if (q->items[i] != NULL) {
            node_print((q->items[i]));
        }
    }
}
