#include "io.h"

#include "code.h"
#include "defines.h"

#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define byte_length(x) ((x / 8) + !!(x % 8))

// read(fd, buf no. of Bytes);
// write(fd, buf no. of Bytes);
static uint8_t buffer[BLOCK]; //GOING TO CONTAIN OUR BITS
static uint64_t bitIndex = 0; // NEED AN INDEX INTO BUFFER
int read_bytes(int infile, uint8_t *buf, int nbytes) { // RETURNS NUM OF BYTES READ
    int total = 0; // TRACKS NUM OF BYTES READ IN SO FAR
    int bytes; // TRACKS BYTES READ BY 1 CALL of READ
    while (total != nbytes) {
        bytes = read(infile, (buf), (nbytes - total));
        total += bytes;
        if (bytes == 0) {
            break;
        }
    }
    return total;
}
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int total = 0; // NUM OF BYTES WRITTEN SO FAR
    int bytes; // NUM OF BYTES WRITTEN EACH ITERATION
    while (total != nbytes) {
        bytes = write(outfile, (buf), (nbytes - total));
        total += bytes;
        if (bytes == 0) {
            break;
        }
    }
    return total;
}
bool small = false;
uint64_t lastBit = ((8 * BLOCK) + 1);
uint64_t size;
bool read_bit(int infile, uint8_t *bit) {
    if (bitIndex == 0) {
        size = read_bytes(infile, buffer, BLOCK);
        uint32_t index = floor(bitIndex / 8);
        *bit = (buffer[index] >> (bitIndex % 8) & 1); // grabbing the bit at given index
        bitIndex += 1;
        return true;
        //printf("size is %lu\n", size);
        /*for (uint64_t i=0; i<size; ++i){
			//printf("buffer[] %d\n", buffer[i]); 
		}
		if (size < BLOCK){
			lastBit = ((8 * size) + 1);
		}*/
    }
    if (bitIndex == size * 8) { // our buffer is full so we fill it again
        size = read_bytes(infile, buffer, BLOCK);
        bitIndex = 0;
        if (size == 0) {
            return false;
        }
    }
    uint32_t index = floor(bitIndex / 8);
    *bit = (buffer[index] >> (bitIndex % 8) & 1); // grabbing the bit at given index
    bitIndex += 1;
    return true;
}
//static uint8_t writeBuf[2] = { 0 };
void write_code(int outfile, Code *c) { // if buffer is full write out contents of buffer
    uint32_t beforeTop = c->top;
    for (uint32_t i = 0; i < (beforeTop); ++i) {
        //uint8_t temp;
        //code_pop_bit(c, &temp);
        //printf("temp %d \n", temp);
        uint32_t index = floor(bitIndex / 8);
        if (c->bits[i] == 1) {
            buffer[index] |= (1 << (bitIndex % 8)); // setting a bit
            if (bitIndex == (BLOCK * 8)) { // BUFFER IS FULL... NOW WE WRITE bytes
                write_bytes(outfile, buffer, BLOCK);
                bitIndex = 0;
            }
        } else {
            buffer[index] &= ~(1 << (bitIndex % 8)); // Clearing a bit
            if (bitIndex == (BLOCK * 8)) { // BUFFER IS FULL... NOW WE WRITE bytes
                write_bytes(outfile, buffer, BLOCK);
                bitIndex = 0;
            }
        }
        bitIndex += 1;
    }
    c->top = beforeTop;
    flush_codes(outfile);
}

void flush_codes(
    int outfile) { // We want to write the rest of the codes in the buffer if it is not full.
    if (bitIndex != 0) {
        uint64_t nbytes = byte_length(bitIndex);
        write_bytes(outfile, buffer, nbytes);
        for (uint64_t i = 0; i < (bitIndex / 8); ++i) {
            buffer[i] = 0;
        }
        bitIndex = 0;
    }
}
