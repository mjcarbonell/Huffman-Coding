#include "defines.h"
#include "encode.h"
#include "header.h"
#include "io.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define OPTIONS "hvui:o:"
#define byte_length(x) ((x / 8) + !!(x % 8))
bool vPrint = false;
int main(int argc, char **argv) {
    char *newFile = malloc(256);
    char *newFileOut = malloc(256);
    for (int i = 0; i < argc; i++) { // Handles arguments
        if (strcmp(argv[i], "-h") == 0) {
            fprintf(stdout, "SYNOPSIS\n");
            fprintf(stdout, "	A Huffman encoder.\n\n");
            fprintf(stdout, "Compresses a file using the HUffman coding algorithm\n");
            fprintf(stdout, "USAGE\n");
            fprintf(stdout, "	./decode [-h] [-i infile] [-o outfile]\n");
            fprintf(stdout, "OPTIONS\n");
            fprintf(stdout, "	-v	Compression statistics.\n");
            fprintf(stdout, "	-h	program usage and help.\n");
            fprintf(stdout, "	-i infile	Input containing graph (default: stdin)\n");
            fprintf(stdout, "	-o outfile	Output of computed path (default: stdout)\n");
        }
        if (strcmp(argv[i], "-i") == 0) {
            strcpy(newFile, argv[i + 1]);
            freopen(newFile, "r", stdin);
        }
        if (strcmp(argv[i], "-o") == 0) {
            strcpy(newFileOut, argv[i + 1]);
            freopen(newFileOut, "w", stdout);
        }
        if (strcmp(argv[i], "-v") == 0) {
            vPrint = true;
        }
    }

    Header h;
    read(0, &h, sizeof(Header)); // Reading the header that we wrote to the outfile
    //printf("this is tree_size %d\n", h.tree_size);
    int treeSize = h.tree_size;
    if (h.magic != MAGIC) {
        exit(1);
    }
    int fileSize = h.file_size;
    fchmod(1, h.permissions); // Set permissions for outfile
    uint8_t treeArray[treeSize];
    for (int i = 0; i < treeSize; ++i) {
        treeArray[i] = 0;
    }
    static uint8_t buffer[BLOCK] = { 0 };
    int bytes;
    bytes = read(0, buffer, treeSize); // reading in the tree dump
    for (int i = 0; i < treeSize; ++i) { // placing tree dump in array
        treeArray[i] = (buffer[i]);
        //printf("%c\n", buffer[i]);
    }
    //printf("treeaarrya[] %d\n", treeArray[0]);
    //printf("buffer[] is %c\n", buffer[treeSize-1]);
    Node *root
        = rebuild_tree(treeSize, treeArray); // We need to rebuild the huffman tree using a stack
    int count = 0;
    uint8_t bit;
    bool found = false;
    Node *cur = root;
    uint8_t readBuf[2] = { 0 };
    lseek(0, treeSize, 0);
    while (read_bit(0, &bit)) {
        if (found == true) {
            if (count % 8 != 0 && count != 0) {
                //printf("ignore\n");
                count += 1;
                //printf("bit is %d\n", bit);
                continue;
            }
            found = false;
        }
        //printf("BIT %d\n", bit);
        if (bit == 0) {
            if (cur->left != NULL) {
                cur = cur->left;
            }
        } else {
            if (cur->right != NULL) {
                cur = cur->right;
            }
        }
        if (cur->left == NULL || cur->right == NULL) { // THIS IS A LEAF NODE
            //printf("LEAF is %c\n", cur->symbol);
            //node_print(cur);
            readBuf[0] = cur->symbol;
            write(1, readBuf, 1);
            //printf("ADDED %d \n", cur->symbol);
            cur = root;
            found = true;
        }
        count += 1;
    }
    
    if (vPrint == true){
    	fprintf(stdout, "Compressed file size: %lu bytes\n", (treeSize + byte_length(count) + sizeof(h))); 
    	int num2 = (treeSize + byte_length(count) + sizeof(h));
    	fprintf(stdout, "Decompressed file size: %lu bytes\n", h.file_size); 
    	fprintf(stdout, "Space saving: %lu", ((fileSize - (num2) / (h.file_size)))); 
    }
    
    
}
