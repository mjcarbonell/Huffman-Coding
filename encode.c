/* 
Created by: Carbonell-Kiamtia, Maxim 
mjcarbon 
May 23rd, 2021

Assignment 6:  Huffman Coding 
CSE 13S, Computer Systems and C Programming 
UC Santa Cruz, Spring 2021

Description: This directory is for the purposes of encoding characters in a file, generating a histogram, generating a huffman tree, and decoding the codes with a reconstructed huffman tree to decode a file back to its orignal state. 
*/
#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define byte_length(x) ((x / 8) + !!(x % 8))
bool vPrint = false;
int leaves = 0;
int statbuf;
uint8_t postArray[BLOCK] = { 0 }; // an array of leaf node symbols
uint64_t count = 0;
void postOrderMain(
    Node *n) { // A function to create a tree dump that organizes unique symbols and interior nodes
    bool isLeaf = true;
    if (n != NULL) { // we only wish to deal with nodes that exist
        if (n->left != NULL) { // traverse down every single possible path from every single node
            isLeaf = false;
            postOrderMain(n->left);
        }
        if (n->right != NULL) {
            isLeaf = false;
            postOrderMain(n->right);
        }
        if (isLeaf == true) {
            postArray[count] = 76;
            count += 1;
            postArray[count] = n->symbol;
            count += 1;
        }
        if (isLeaf == false) {
            postArray[count] = 73;
            count += 1;
        }
    }
}
int main(int argc, char **argv) { //NOTE stdin == 0 and stdout == 1
    /* a histogram to hold the count of characters 0,...,256; it is
   	*initialized to zero on every element */
    char *newFile = malloc(256);
    char *newFileOut = malloc(256);
    uint64_t hist[ALPHABET] = { 0 };
    Code table[ALPHABET];
    static uint8_t buffer[BLOCK] = { 0 }; //GOING TO CONTAIN OUR BYTES
    int k; // loop counter from 0 to 256
    uint64_t total = 0;
    int bytes;
    for (int i = 0; i < argc; i++) { // Handles arguments
        if (strcmp(argv[i], "-h") == 0) {
            fprintf(stdout, "SYNOPSIS\n");
            fprintf(stdout, "	A Huffman encoder.\n\n");
            fprintf(stdout, "Compresses a file using the HUffman coding algorithm\n");
            fprintf(stdout, "USAGE\n");
            fprintf(stdout, "	./encode [-h] [-i infile] [-o outfile]\n");
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

    while ((bytes = read(0, (buffer), BLOCK))  > 0) { // as long as we keep reading a bytes we will increment the amount of occurences of each character in a file.
        total += bytes;
        for (int i = 0; i < bytes; ++i) {
            hist[buffer[i]] += 1;
        }
    }
    for (k = 0; k < 256; k++) {
        if (hist[k] > 0) {
            leaves += 1; // We must know the amount of leaves/unique symbols in our file
            //printf("char %c: %lu times\n", k, hist[k]);
        }
    }
    Node *root = build_tree(hist); // Building our huffman tree
    build_codes(root, table); // creating a code for each unique symbol
    Header h;
    struct stat sb;
    h.magic = MAGIC;
    fstat(0, &sb); // Grabbing stats of infile
    fchmod(1, sb.st_mode); // Setting permissions of outfile w/ permissions of infile
    h.permissions = sb.st_mode;
    h.tree_size = ((leaves * 3) - 1);
    h.file_size = sb.st_size;
    write_bytes(1, ((uint8_t *) &h), sizeof(h));
    postOrderMain(root); // Creating a tree dump with a post order function
    uint64_t totalWrite = 0; // NUM OF BYTES WRITTEN SO FAR
    while (totalWrite != count) { // WRITING TREE DUMP TO OUTFILE
        bytes = write(1, postArray, (count - totalWrite));
        totalWrite += bytes;
        if (bytes == 0) {
            break;
        }
    }
    lseek(0, 0, 0);
    int codeCount = 0;
    while ((bytes = read(0, (buffer), BLOCK)) > 0) { // as long as we keep reading a bytes
        total += bytes;
        for (int i = 0; i < bytes; ++i) { // We are writing codes that match with the order and frequency of the original file
            for (k = 0; k < 256; ++k) {
                if (k == buffer[i]) {
                    //printf("CODE for %c \n", k);
                    //code_print(&table[k]);
                    write_code(1, &table[k]);
                    codeCount += 1;
                }
            }
        }
    }
    struct stat sc;
    fstat(1, &sc);
    if (vPrint == true){
    	fprintf(stdout, "Uncompressed file size: %lu bytes\n", h.file_size); 
    	fprintf(stdout, "Compressed file size: %ld bytes\n", (sizeof(h) + totalWrite + byte_length(codeCount))); 
    	fprintf(stdout, "Space saving: %lu", (h.file_size - (sizeof(h) + totalWrite + byte_length(codeCount))) / (h.file_size)); 
    }

    close(1);
    close(0);
}
























