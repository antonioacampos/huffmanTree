#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX_SIZE 256

typedef struct HuffmanNode {
  char character;
  int frequency;
  struct HuffmanNode *left, *right;
} HuffmanNode;

typedef struct {
  HuffmanNode *nodes[MAX_SIZE];
  int size;
} PriorityQueue;

HuffmanNode *createNode(char character, int frequency);
PriorityQueue *createPriorityQueue();
void insertNode(PriorityQueue *pq, HuffmanNode *node);
HuffmanNode *extractMin(PriorityQueue *pq);
HuffmanNode *buildTrie(int *frequencies);
void buildCodeHelper(HuffmanNode *x, char *code, int depth,
                     char codes[MAX_SIZE][MAX_SIZE]);
void buildCode(HuffmanNode *root, char codes[MAX_SIZE][MAX_SIZE]);
void printCodes(char codes[MAX_SIZE][MAX_SIZE], int *frequencies);
void encode(char *text, char codes[MAX_SIZE][MAX_SIZE], char *encodedText);
void decode(char *encodedText, HuffmanNode *root, char *decodedText);
void freeTree(HuffmanNode *node);

#endif
