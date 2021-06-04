#include "code.h"

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

Code code_init(void) {
    //uint8_t cArray[MAX_CODE_SIZE];
    /*for (uint64_t i=0; i < MAX_CODE_SIZE; ++i){
		cArray[i] = 0;
	}*/
    //Code c = {0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
    //Code c = {0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
    Code c;
    for (uint64_t i = 0; i < MAX_CODE_SIZE; ++i) {
        c.bits[i] = 0;
    }
    c.top = 0;

    return c;
}

uint32_t code_size(Code *c) {
    return (c->top);
}

bool code_empty(Code *c) {
    if (c->top == 0) {
        return true;
    }
    return false;
}

bool code_full(Code *c) {
    if ((c->top) == MAX_CODE_SIZE) {
        return true;
    }
    return false;
}

bool code_push_bit(Code *c, uint8_t bit) {
    if ((c->top) == MAX_CODE_SIZE) { // Code is full
        return false;
    }
    c->bits[c->top] = bit;
    c->top += 1;
    return true;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
    if (c->top == 0) { // code is empty
        //printf("FALSE\n");
        return false;
    }
    c->top -= 1;
    *bit = (c->bits[c->top]);
    return true;
}

void code_print(Code *c) {
    uint32_t cSize = code_size(c);
    for (uint32_t i = 0; i < cSize; ++i) {
        printf("%d  \n", c->bits[i]);
    }
}
