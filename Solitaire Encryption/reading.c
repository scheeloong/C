#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for toupper to conver str to uppercase
                    // for isalpha to remove non letters
#include "reading.h"
#include "card.h"

// This function takes in a pointer to a file,
// which carries information about a deck.
// It reads all cards from the deck file and
// returns them as a circular linked list.
Card *readDeck (char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Can't open file in readDeck\n");
        exit(EXIT_FAILURE);
    }
    int val = 0; // initializing val
    int temp = 0; // to know when to stop scanning
    fscanf(fp, "%d", &val);
    Card *cardlist = newCard(val);
    while ( fscanf(fp, "%d", &val) && val != temp  )
    {
        temp = val;
        cardlist = addTailCard(cardlist, val);
    }
    //close the file
    fclose(fp);
    return cardlist;
}

// Read and store a line from message fp into str.
// Store at most n characters.
// Discard non-letters and
// convert all letters to uppercase.
// Str will always be a valid string.
// and returns the number of new characters stored.
int readLine(FILE *fp, char str[], int n)
{
    if (fp == NULL)
    {
        perror("Can't open file in readLine\n");
        exit(EXIT_FAILURE);
    }
    // clear the str given to start storing from index 0.
    strcpy(str,""); // make str an empty str
    int count = 0; // to return number of characters stored.
    char c = ' ';
    char temp[2];
    while ( c != '\n' && count < n)
    {
        count++;
        fscanf(fp, "%c", &c);
        // isalpha returns true if character read is a letter (doesn't include spaces)
        if (isalpha(c))
        {
            temp[0] = c;
            temp[1] = '\0';
            strcat(str, temp);
        }
    }
    // convert all letters to uppercase
    int i = 0;
    for ( ; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }
    // get length of string to return.
    if (strcmp(str, "") != 0) count = strlen(str);
    else count = 0;
    return count;
}
