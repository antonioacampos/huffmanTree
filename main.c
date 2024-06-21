#include "huffman.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 256

void clearFrequencies(int *frequencies) {
  for (int i = 0; i < MAX_SIZE; i++) {
    frequencies[i] = 0;
  }
}

int countDistinctCharacters(char *text) {
  int count = 0;
  int found[MAX_SIZE] = {0};
  for (int i = 0; text[i]; i++) {
    if (!found[(unsigned char)text[i]]) {
      found[(unsigned char)text[i]] = 1;
      count++;
    }
  }
  return count;
}

void calculateCompressionRates(char *text, char *encodedText) {
  int originalBits = strlen(text) * 8;
  int encodedBits = strlen(encodedText);
  int distinctChars = countDistinctCharacters(text);
  int minBits = (int)ceil(log2(distinctChars)) * strlen(text);

  double compressionRate8Bit = (double)encodedBits / originalBits * 100;
  double compressionRateMinBit = (double)encodedBits / minBits * 100;

  printf("Taxa de compactação (em relação a 8 bits): %.2f%%\n",
         compressionRate8Bit);
  printf("Taxa de compactação (em relação ao mínimo de bits): %.2f%%\n",
         compressionRateMinBit);
}

int main() {
  int frequencies[MAX_SIZE] = {0};
  char codes[MAX_SIZE][MAX_SIZE] = {0};
  HuffmanNode *root = NULL;
  char command[MAX_SIZE], character;
  int frequency;

  while (1) {
    printf("-> ");
    scanf("%s", command);
    if (strcmp(command, "add") == 0) {
      scanf(" %c %d", &character, &frequency);
      frequencies[(unsigned char)character] = frequency;
    } else if (strcmp(command, "calc") == 0) {
      if (root) {
        freeTree(root);
      }
      root = buildTrie(frequencies);
      buildCode(root, codes);
    } else if (strcmp(command, "print") == 0) {
      printCodes(codes, frequencies);
    } else if (strcmp(command, "cod") == 0) {
      char text[MAX_SIZE], encodedText[MAX_SIZE];
      scanf("%s", text);
      encode(text, codes, encodedText);
      printf("%s\n", encodedText);
      calculateCompressionRates(text, encodedText);
    } else if (strcmp(command, "dec") == 0) {
      char encodedText[MAX_SIZE], decodedText[MAX_SIZE];
      scanf("%s", encodedText);
      decode(encodedText, root, decodedText);
      printf("%s\n", decodedText);
    } else if (strcmp(command, "clear") == 0) {
      clearFrequencies(frequencies);
      memset(codes, 0, sizeof(codes));
      if (root) {
        freeTree(root);
        root = NULL;
      }
    } else if (strcmp(command, "exit") == 0) {
      break;
    } else {
      printf("Comando inválido!\n");
    }
  }

  if (root) {
    freeTree(root);
  }

  return 0;
}
