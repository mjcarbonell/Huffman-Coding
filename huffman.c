#include "huffman.h"

#include "code.h"
#include "defines.h"
#include "encode.h"
#include "node.h"
#include "pq.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
Code tempTable[ALPHABET];

void tree(PriorityQueue *
        p) { // Function to construct a huffman tree. We dequeue the highest priority nodes and join them until we have one root node.
    //Dequeue two and join
    //Check if there is one node left
    uint32_t pSize = pq_size(p);
    //printf("pSize is %d\n", pSize);
    while (pSize > 1) {

        Node *temp1;
        Node *temp2;
        dequeue(p, &temp1);
        dequeue(p, &temp2);
        Node *cJoined = node_join(temp1, temp2);
        //printf("children are here\n");
        //printf("%c\n", temp1->symbol);
        //printf("%c\n", temp2->symbol);
        //printf("cJoined is here\n");
        //node_print(cJoined);
        enqueue(p, cJoined);
        pSize = pq_size(p);
    }
    return;
}
Code c;
bool firstTime = true;
uint32_t cSize = 0;
uint32_t tableSizes[ALPHABET] = { 0 };
void postOrder(Node *n,
    Code table
        [static ALPHABET]) { // We create a code and traverse the tree to generate a unique code for each unique symbol.
    bool isLeaf = true;
    uint8_t temp;
    if (firstTime == true) {
        firstTime = false;
        c = code_init();
    }
    if (n != NULL) {
        if (n->left != NULL) { // if we go left then add a 0 to the path/code.
            isLeaf = false;
            code_push_bit(&c, 0);
            cSize = code_size(&c);
            postOrder(n->left, table);
        }
        if (n->right != NULL) { // if we go right then add a 1 to the path/code
            isLeaf = false;
            code_push_bit(&c, 1);
            cSize = code_size(&c);
            postOrder(n->right, table);
        }
        if (isLeaf == true) {
            table[n->symbol] = c; // once we have found a unique symbol... Add it to the codeTable
            tableSizes[n->symbol] = cSize;
        }
        //node_print(n);
        code_pop_bit(&c,
            &temp); // once we have nowwhere else to go we must pop a bit to update our path... We are essentially going backwards.
        cSize = code_size(&c);
    }
}
Node *build_tree(uint64_t hist
        [static ALPHABET]) { // Given a histogram we will generate nodes for each unqique symbol with their frequencies in the file
    // Create nodes from histogram
    Node *n;
    PriorityQueue *p = pq_create(256);
    for (int k = 0; k < 256; k++) {
        if (hist[k] > 0) {
            n = node_create(k, hist[k]);
            enqueue(p, n);
        }
    }
    //printf("This is tree \n");
    //pq_print(p);
    //printf("AFTER TREE\n");
    tree(p);
    Node *root;
    dequeue(p, &root);
    return root;
}

void build_codes(Node *root,
    Code table
        [static ALPHABET]) { // We wish to generate unique codes for each unique symbol in the file using a post order method.
    firstTime = true;
    postOrder(root, table);
}

Node *rebuild_tree(uint16_t nbytes,
    uint8_t tree[static nbytes]) { // We wish to rebuild a huffman tree given a tree dump
    Stack *s = stack_create(nbytes);
    Node *result;
    for (int i = 0; i < nbytes; ++i) {
        if (tree[i]
            == 76) { // 76 equates to an "L" which means that the character after it is a unique symbol
            Node *n = node_create(tree[i + 1], 1);
            stack_push(s, n);
        }
        if (tree[i]
            == 73) { // 73 equates to an I so we pop the right and left nodes from the stack and join them
            Node *right;
            Node *left;
            Node *joined;
            stack_pop(s, &right);
            stack_pop(s, &left);
            joined = node_join(left, right);
            stack_push(s, joined);
        }
    }
    //printf("STACK \n");
    //stack_print(s);
    stack_pop(s, &result);
    return result; // We want to return ultimate root node
}

void delete_tree(Node **root) {
    if (*root) {
        //delete_tree(*(root->left));
        //delete_tree(*(root->right));
        //free(*root);
        node_delete(&(*root));
        *root = NULL;
    }
}
