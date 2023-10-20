Maxim Joel Carbonell-Kiamtia
mjcarbon


---------------------
DESCRIPTION: This directory is for the purposes of encoding characters in a file, generating a histogram, generating a huffman tree, and decoding the codes with a reconstructed huffman tree to decode a file back to its orignal state. 

1. Enter "make" in the terminal
2. Enter "./encode" -[INSERT COMMANDs HERE]". To insert an input file for the program to read user must type " < [insert input file here]" or "-i [insert input file here]" and if the user wishes to use an output file other than stdout they must type "-o [insert input file here]".  Possible commands for ./encode i.e. "-h". Possible commands ofr ./decode i.e. "-h" for help message and "-v" for printing statistiics. 
---------------------
FILES
DESIGN.pdf
Makefile
README.md
encode.c 
encode.h
decode.c
code.c 
code.h
defines.h 
entropy.c 
header.h 
huffman.c 
huffman.h 
io.c 
io.h 
node.c 
node.h 
pq.c 
pq.h 
stack.c 
stack.h 

-----------------------
encode.c contains the c program to perform all of the logic needed to encode a file containing characters. It reads with a buffer to generate a histogram, codes, and a huffman tree. 

decode.c contains the c progra mto read in the codes and huffman tree given a tree dump. Ultimately, it reads bit by bit until it has decoded the file.

code.c contains the logic needed to maintain readable code.

huffman.c contains the logic needed to construct and reconstruct a huffman tree as well as building codes for each unique symbol in a file. 

io.c contains the logic needed to write code to a file, read bytes, and write bytes. 

node.c contains the logic needed to maintain nodes and join nodes together into one parent node. 

pq.c contains the logic needed to maintain a huffman tree. It organizes nodes by priority. 

stack.c contains logic needed to hold nodes and is used to rebuild a huffman tree. 

DESIGN.pdf contains a walkthrough or behind the scenes process to create or recreate this directory. 
---------------------



















