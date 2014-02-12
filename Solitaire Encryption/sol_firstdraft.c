#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for getopt
#include <limits.h> // for PATH_MAX
#include "card.h"
#include "reading.h"
#define MAX_LEN 80 //Maximum length of message

// Given the head of a circular linked list of cards,
// generate the next keystream value, (one round of solitaire)
// and store it in the memory pointed to by bit.
Card *nextRound (Card *deck, int *bit)
{
    // 1. Swap 27 once
    deck = swapAfter(deck, 27);
    // 2. Swap 28 twice.
    deck = swapAfter(deck, 28);
    deck = swapAfter(deck, 28);
    // 3. Perform Triple Cut
    // determine which Joker is higher
    // by comparing their positions
    int Joker1 = cardPos(deck, 27);
    int Joker2 = cardPos(deck, 28);
    // if 28 is above 27 in the deck
    if (Joker1 > Joker2)
    {
        deck = tripleCut(deck, 28, 27);
    }
    // if 27 is above 28 in the deck.
    else  // if Joker2 > Joker 1
    {
        deck = tripleCut(deck, 27, 28);
    }
    // 4. Insert cards according to value of bottom
    //    card of deck.
    int val = cardVal(deck, 28);
    deck = insertBottom(deck, val);
    // 5. Get the keystream value
    // Count down to the deck according
    // to the value of the top card
    int count = cardVal(deck, 1);
    if (count == 28) count = 27;
    // get the value of the count+1 card
    val = cardVal(deck, count + 1);
    // if the value is a joker
    if (val == 27 || val == 28)
    { // redo round from step 1 using recursion
        deck = nextRound(deck, bit);
    }
    // use else statement to prevent recursion
    // from overriding value of bit.
    else { *bit = val;}
    // return the deck for next round of solitaire.
    return deck;
}

// Given a deck filename, a message filename, and a mode
// ('e' or 'd'), encrypt or decrypt the messages using the deck.
// Output the processed messages on standard output, one per line.
void process (char *deckfname, char *msgfname, char mode)
{
    Card *cardlist;
    cardlist = readDeck(deckfname);
    if (cardlist == NULL)
    {
        perror("Can't read deck file\n");
        exit(EXIT_FAILURE);
    }
    // convert msgfname to file
    FILE *input_file;
    input_file = fopen(msgfname, "r");
    if (input_file == NULL)
    {
        perror("Can't open message file in process\n");
        exit(EXIT_FAILURE);
    }

    // initialize i to get number of chars read onto str.
    int i = 1;
    // initialize str to read each line from the file.
    char *str = malloc(MAX_LEN + 1);
    strcpy(str, "");
    // Variables for generating keystream values
    int *bit = malloc(sizeof(int));
    *bit = 0;
    int j = 0; // to index the string
    int num = 0; // to represent digit values of characters.
    int length = 0; // to calculate length of str.
    // if encryption mode.
    if (mode == 'e')
    {   // encrypt message
        // loop until whole file is read line by line
        while ( i )
        {
            i = readLine(input_file, str, MAX_LEN);
            length = strlen(str);
            for ( j=0 ; str[j] != '\0' ; j++)
            {
                num = str[j] - 'A' + 1; // change into number representation.
              //  printf("the value of j in letter is %c and in number is %d\n", str[j], num);
                cardlist = nextRound (cardlist, bit);
              //  printf("bit is value %d\n", *bit);
                num += *bit; if (num > 26) num -= 26;
                num += ('A' - 1); // convert back to proper letter.
                printf("%c", num);
            }
            printf("\n"); // print new line.
        }
    }
    else if (mode == 'd')
    {   // decrypt message
        // loop until whole file is read line by line
        while ( i )
        {
            i = readLine(input_file, str, MAX_LEN);
            length = strlen(str);
            for ( j=0 ; str[j] != '\0' ; j++)
            {
                num = str[j] - 'A' + 1; // change into number representation.
              //  printf("the value of j in letter is %c and in number is %d\n", str[j], num);
                cardlist = nextRound (cardlist, bit);
              //  printf("bit is value %d\n", *bit);
                if (num <= *bit) num += 26;
                num -= *bit;
                num += ('A' - 1); // convert back to proper letter.
                printf("%c", num);
            }
            printf("\n"); // print new line.
        }
    }
    // free the memory allocated
    deallocateCards(cardlist);
    free(bit);
    free(str);
}

// The main function processes the commandline-options
// i) -E for encrypt or -D for decrypt
// ii) -d<deckfile>  // specifies the name of the deck file
// iii) -m<messagefile> // specifies the name of the message file
int main(int argc, char *argv[])
{
    int opt; // to read the arguments given.
    // variables to read filenames and command mode
    char deckfname[PATH_MAX] = {0}, msgfname[PATH_MAX] = {0};
    char mode = 'z'; // initialize to no mode.
    // read commands till there are no more option characters
    // getopt returns -1 when there are no more option characters.
    while ((opt = getopt(argc, argv,"DE d:m: " )) != -1)
    {
        switch (opt)
        {
            // Encrypt mode given
            case 'E':
                mode = 'e';
                break;
            // Decrypt mode given
            case 'D':
                mode = 'd';
                break;
            // deckfname is given
            case 'd':
                strcpy(deckfname,optarg);
                break;
            // msgfname is given
            case 'm':
                strcpy(msgfname,optarg);
                break;
            case '?': // if unknown command
                if (optopt == 'c')
                    fprintf(stderr, "Option -%c requires an argument. \n", optopt);
                else if ( isprint (optopt)) // checks if optopt is a printable character
                        fprintf(stderr,"Unknown option -%c \n", optopt);
                else
                    perror("Unknown option character \n");
                exit(EXIT_FAILURE);
                break;
            default:
                abort();
        }
    }
    if (mode == 'z')
    {
        perror("ERROR!! No mode given\n");
        exit(EXIT_FAILURE);
    }
    //Obtain and validate commandline args
    process (deckfname, msgfname, mode);
    return 0;
}

/*// for testing
int main2(void)
{
    char deckfname[PATH_MAX] = {0}, msgfname[PATH_MAX] = {0};
    char mode = 'd';
    process("deck1.txt", "secret.txt", mode);
    return 0;
}

int main3(void)
{
    char deckfname[PATH_MAX] = {0}, msgfname[PATH_MAX] = {0};
    char mode = 'e';
    Card *cardlist = readDeck("deck1.txt");
    Card *curr;
    printCardInfo(cardlist);
    curr = findCard(cardlist, 1); // note that findcard returns the pointer to the number before in deck.
    cardlist = swapAfter(cardlist, curr->next->val); // that's why need next.
    printCardInfo(cardlist);
    int cardpos = 0;
    cardpos = cardPos(cardlist, 26);
    printf("The card position is %d\n", cardpos);
    cardlist = tripleCut(cardlist, 28, 27);
    printCardInfo(cardlist);
    int cardval = 0;
    cardval = cardVal(cardlist, 27);
    printf("The card value at position is %d\n", cardval);
    cardlist = insertBottom(cardlist, 5);
    printCardInfo(cardlist);
    // remove the deck and free it's memory.
   // deallocateCards(cardlist);
   int *bit = malloc(sizeof(int));
   *bit = 0;
   cardlist = nextRound(cardlist, bit);
    printCardInfo(cardlist);
    printf("the generated key stream value is %d\n", *bit);

    FILE *fp;
    fp = fopen("messages.txt", "r");
    if (fp == NULL)
    {
        perror("Can't open file in message.txt\n");
        exit(EXIT_FAILURE);
    }
    char *longstr[MAX_LEN+1];
    *longstr = "haha";
    int num_return = 0;
    num_return = readLine(fp, longstr, MAX_LEN);
    printf("The num_return is %d\n", num_return);
    printf("The string is : %s", longstr);
    return 0;
}
//*/
