#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HuffmanNode *createNode(char character, int frequency) {
  HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
  if (node) {
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
  } else {
    perror("Failed to allocate memory for HuffmanNode");
    exit(EXIT_FAILURE);
  }
  return node;
}

PriorityQueue *createPriorityQueue() {
  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  if (pq) {
    pq->size = 0;
  } else {
    perror("Failed to allocate memory for PriorityQueue");
    exit(EXIT_FAILURE);
  }
  return pq;
}
void swap(HuffmanNode **a, HuffmanNode **b) {
  HuffmanNode *t = *a;
  *a = *b;
  *b = t;
}

void insertNode(PriorityQueue *pq, HuffmanNode *node) {
  int i = pq->size++;
  pq->nodes[i] = node;

  while (i && (pq->nodes[i]->frequency < pq->nodes[(i - 1) / 2]->frequency ||
               (pq->nodes[i]->frequency == pq->nodes[(i - 1) / 2]->frequency &&
                pq->nodes[i]->character < pq->nodes[(i - 1) / 2]->character))) {
    swap(&pq->nodes[i], &pq->nodes[(i - 1) / 2]);
    i = (i - 1) / 2;
  }
}

HuffmanNode *extractMin(PriorityQueue *pq) {
  HuffmanNode *min = pq->nodes[0];
  pq->nodes[0] = pq->nodes[--pq->size];
  int i = 0, left, right, smallest;
  while (1) {
    left = 2 * i + 1;
    right = 2 * i + 2;
    smallest = i;
    if (left < pq->size &&
        pq->nodes[left]->frequency < pq->nodes[smallest]->frequency)
      smallest = left;
    if (right < pq->size &&
        pq->nodes[right]->frequency < pq->nodes[smallest]->frequency)
      smallest = right;
    if (smallest == i)
      break;
    HuffmanNode *temp = pq->nodes[i];
    pq->nodes[i] = pq->nodes[smallest];
    pq->nodes[smallest] = temp;
    i = smallest;
  }
  return min;
}

HuffmanNode *buildTrie(int *frequencies) {
  PriorityQueue *pq = createPriorityQueue();
  for (int i = 0; i < MAX_SIZE; i++) {
    if (frequencies[i] > 0) {
      insertNode(pq, createNode((char)i, frequencies[i]));
    }
  }

  while (pq->size > 1) {
    HuffmanNode *left = extractMin(pq);
    HuffmanNode *right = extractMin(pq);
    HuffmanNode *parent = createNode('\0', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    insertNode(pq, parent);
  }

  HuffmanNode *root = extractMin(pq);
  free(pq);
  return root;
}

void buildCodeHelper(HuffmanNode *x, char *code, int depth,
                     char codes[MAX_SIZE][MAX_SIZE]) {
  if (!x->left && !x->right) {
    code[depth] = '\0';
    strcpy(codes[(unsigned char)x->character], code);
    return;
  }
  if (x->left) {
    code[depth] = '0';
    buildCodeHelper(x->left, code, depth + 1, codes);
  }
  if (x->right) {
    code[depth] = '1';
    buildCodeHelper(x->right, code, depth + 1, codes);
  }
}

void buildCode(HuffmanNode *root, char codes[MAX_SIZE][MAX_SIZE]) {
  char code[MAX_SIZE];
  buildCodeHelper(root, code, 0, codes);
}

void printCodes(char codes[MAX_SIZE][MAX_SIZE], int *frequencies) {
  for (int i = 0; i < MAX_SIZE; i++) {
    if (codes[i][0] != '\0') {
      printf("%c com frequencia %d codificado como %s\n", i, frequencies[i],
             codes[i]);
    }
  }
}

void encode(char *text, char codes[MAX_SIZE][MAX_SIZE], char *encodedText) {
  encodedText[0] = '\0';
  for (int i = 0; text[i]; i++) {
    strcat(encodedText, codes[(unsigned char)text[i]]);
  }
}

void decode(char *encodedText, HuffmanNode *root, char *decodedText) {
  HuffmanNode *current = root;
  int index = 0;
  for (int i = 0; encodedText[i]; i++) {
    if (encodedText[i] == '0')
      current = current->left;
    else
      current = current->right;

    if (!current->left && !current->right) {
      decodedText[index++] = current->character;
      current = root;
    }
  }
  decodedText[index] = '\0';
}

void freeTree(HuffmanNode *node) {
  if (!node)
    return;
  freeTree(node->left);
  freeTree(node->right);
  free(node);
}
