#include "node.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {

    Node *n = (Node *) malloc(sizeof(Node));
    assert(n);
    n->symbol = symbol;
    n->frequency = frequency;
    return n;
}
void node_delete(Node **n) {
    free(*n);
    *n = NULL;
    return;
}
Node *node_join(
    Node *left, Node *right) { // we wish to join two nodes only with unique symbol "$" or 36.
    //joins left and right child and returns a new parent node
    uint8_t newSym = 36;
    uint64_t newFreq
        = ((left->frequency)
            + (right->frequency)); // the frequncy of the parent node will be the summation of the chiuldren's frequnecy
    Node *parent = node_create(newSym, newFreq);
    parent->left = left;
    parent->right = right;
    return parent;
}

void node_print(Node *n) { // print the symbol and frequnecy of the node
    printf("node symbol %d and freq %lu\n", n->symbol, n->frequency);
}
