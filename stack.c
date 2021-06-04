#include "stack.h"

#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack { // Stack is very similar to the priority queue... We have a double pointer of Nodes
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = calloc(capacity, sizeof(Node *));
        return s;
    }
    return NULL;
}

void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        for (uint32_t i = 0; i < (*s)->top; ++i) {
            node_delete(&(*s)->items[i]);
        }
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
}

bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

uint32_t stack_size(Stack *s) {
    return (s->top);
}

bool stack_push(Stack *s,
    Node *
        n) { // Rather than placing a node according to its frequnecy... we just wish to place a ndoe at the top of the stack
    if (s->top == s->capacity) {
        return false;
    }
    s->items[s->top] = n;
    s->top += 1;
    return true;
}

bool stack_pop(
    Stack *s, Node **n) { // Likewise, we only wish to retrieve the node at the top of the stack
    if (s->top == 0) {
        return false;
    }
    s->top -= 1;
    *n = s->items[s->top];
    //node_delete(&(s->items[s->top]));
    return true;
}

void stack_print(Stack *s) { // print all nodes in stack.
    for (uint64_t i = 0; i < (s->top); ++i) {
        node_print(s->items[i]);
    }
}
