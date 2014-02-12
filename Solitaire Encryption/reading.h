#ifndef READING_H
#define READING_H
#include "card.h"

// Read all cards from deck filename and
// return them as a circular linked list.
Card *readDeck (char *filename);

// Read and store a line from message fp into str.
// Store at most n characters.
// Discard non-letters and
// convert all letters to uppercase.
int readLine(FILE *fp, char str[], int n);

#endif
