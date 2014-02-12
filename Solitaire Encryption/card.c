#include <stdio.h>
#include <stdlib.h>
#include "card.h"

// Given the head of a circular linked list of cards,
// print each card's value on the same line separated by spaces.
// Print a newline after printing all card's values.
void printCardInfo (Card *cardlist)
{
    // if cardlist is empty
    // do nothing.
    if (!cardlist)
        return;
    Card *head = cardlist;
    do
    // print the values of each card
    // while the cardlist is not back to
    // its beginning
    {
        printf ("%d ", cardlist->val);
        cardlist = cardlist->next;
    } while (cardlist != head);
      // prints a newline once all the values
      // have been printed out.
    printf ("\n");
}

// Create a new circular linked list of cards
// with one card of value val. Both the head and
// that one card with value val points to each other.
// Return the pointer to the head.
Card *newCard (int val)
{
    // Initialize two pointers.
    // one as a head pointer
    // whereas the other as a pointer to the
    // first card in the circular linked list
    Card *cardlist = NULL;
    Card *newCard = malloc(sizeof(Card));
    if (newCard == NULL)
    {
        perror("newCard failed to malloc");
        exit(EXIT_FAILURE);
    }
    // initialize the value of the newCard to val
    newCard->val = val;
    // make both pointers point to each other
    newCard->next = newCard;
    cardlist = newCard;
    // return pointer to the head
    // of the new circular linked list.
    return cardlist;
}

// Given a pointer to the head of a circular linked list,
// and the value of a new card,
// add the new card to the END of cardlist.
// and returns the head of the circular linked list.
Card *addTailCard (Card *cardlist, int val)
{
    Card *curr = NULL;
    curr = cardlist;
    while (curr->next != cardlist)
    {
        curr = curr->next;
    }
    // now curr is pointing to the last card.
    // and curr->next is pointing to the first card
    // initialize newCard with given value.
    Card *newCard = malloc(sizeof(Card));
    if (newCard == NULL)
    {
        perror("addTailCard failed to malloc");
        exit(EXIT_FAILURE);
    }
    newCard->val = val;
    // make newCard point to first Card
    newCard->next = curr->next;
    // make previous card point to
    // current last card.
    curr->next = newCard;
    // return head that still points to first card.
    return cardlist;
}

// A helper function (not to be used outside card.c)
// Given the head of a circular linked list of cards,
// and the value of a card, return the card prior to that card.
// The next pointer of the returned card is card with value val.
// If val is not found, return NULL.
Card *findCard (Card *cardlist, int val)
{
    Card *prev = NULL;
    Card *curr = cardlist;
    // if value is at first card
     if (curr->val == val)
     {
         while (curr->next != cardlist)
         {
             curr = curr->next;
         }
         // here, curr is pointing to the last card
         return curr;
     }
    // while the value is not found
    // and it has not looped through the whole deck.
    while( curr->val != val && curr->next != cardlist )
    {
        // let prev be the card before curr.
        prev = curr;
        // curr moves on to the next card in the
        // circular linked list deck.
        curr = curr->next;
    }
    // Here, either the value is found
    // or we are at the last card of the deck.
    // Check if the current card
    // or the last card is the value val
    // if it is, return prev.
    if (curr->val == val) return prev;
    // if it is not, the value is not found.
    return NULL;
}

// Given the head of a circular linked list of cards,
// and the value of an existing card,
// swap that card with the next card in cardlist.
// and return the head to the circular linked list of cards
Card *swapAfter (Card *cardlist, int val)
{
    Card *curr = cardlist;
    // initialize prev just in case
    // value is in the first card.
    // then prev will be pointing
    // to the last card.
    Card *prev = findCard(cardlist, cardlist->val);
    while ( curr->val != val)
    {
        prev = curr;
        curr = curr->next;
    }
    // if the last card swaps the top card instead.
    if (curr->next == cardlist)
    {
        // return the header that points to the new top card
        cardlist = curr;
    }
    // if the top card swaps with the 2nd card
    else if (prev->next == cardlist)
    {
        // return cardlist pointing to the new first card.
        cardlist = curr->next;
    }
    prev->next = curr->next;
    curr->next = curr->next->next;
    prev->next->next = curr;
    return cardlist;
}

// Given the head of a circular linked list of cards,
// and the value of an existing card,
// return the position of that card
// in the circular linked list.
// Top card => position 1
// Bottom card => position 28
int cardPos (Card *cardlist, int val)
{
    // variable to keep track of
    // position of card
    int pos = 1;
    Card *curr = cardlist;
    while( curr->val != val)
    {
        curr = curr->next;
        pos++;
    }
    return pos;
}

// Given the head of a circular linked list of cards
// and the values of the top and bottom jokers (27 or 28)
// perform a triple cut.
// Triple Cut = swap cards above the top joker,
// with cards below the second joker.
Card *tripleCut (Card *cardlist, int top, int bot)
{
    // Initialize pointers to point to
    // the first and last card above the top joker
    // and first and last card below the bottom joker.
    Card *begin_top = NULL, *end_top = NULL;
    Card *begin_bot = NULL, *end_bot = NULL;
    // a pointer to the bottom joker.
    Card *joker_bot = NULL;

    // begin_top points to the first card.
    begin_top = cardlist;
    // end_top points to the card before the top joker.
    end_top = findCard(cardlist, top);
    // joker_bot points to the bottom joker.
    joker_bot = findCard(cardlist, bot);
    joker_bot = joker_bot->next;
    // begin_bot points to the card after the bottom joker.
    begin_bot = joker_bot->next;
    //end_bot points to the last card.
    end_bot = findCard(cardlist, cardlist->val);

    // initialize variables to get the positions of both jokers
    // for case handling.
    int top_pos = cardPos(cardlist, top);
    int bot_pos = cardPos(cardlist, bot);

    // Case 1: top joker is first card and bottom joker is last card
    if (top_pos == 1 && bot_pos == 28)
    {
        // do nothing
        return cardlist;
    }

    // Case 2 : top joker is the first card.
    else if (top_pos == 1)
    {
        // change the position of the first card
        cardlist = begin_bot;
        return cardlist;
    }
    // Case 3: bottom joker is the last card.
    else if (bot_pos == 28)
    {
        // make top joker the first card.
        cardlist = end_top->next;
        return cardlist;
    }
    // The ELSE statement covers:
    // Case 4: top joker is the 2nd card
    // Case 5: bottom joker is the 2nd last card
    // Case 6: both jokers are not on the top 2 or bottom 2 positions on the deck.
    else
    {
        // update cardlist to the new top card which is begin_bot
        cardlist = begin_bot;
        // point the last card to the top joker.
        end_bot->next = end_top->next;
        // point the end_top which will be the last card
        // to the new top_card.
        end_top->next = cardlist;
        joker_bot->next = begin_top;
    }
    return cardlist;
}

// Given the head of a circular linked list of cards,
// and a position, return value of card
// at that position. Top card => position 1
int cardVal(Card *cardlist, int pos)
{
    int count = 1;
    Card *curr = cardlist;
    while(count != pos)
    {
        count++;
        curr = curr->next;
    }
    return curr->val;
}

// Given the head of a circular linked list of cards,
// insert num top cards above the bottom-most card
Card *insertBottom (Card *cardlist, int num)
{
    // special case where bottom card number is 28.
    if (num == 28)
    {
        num = 27;
    }
    // initialize pointers to bottom card
    // and the card before bottom card.
    Card *bot_card = findCard(cardlist, cardlist->val);
    Card *bef_bot = findCard (cardlist, bot_card->val);

    // initialize pointer to the card at position num.
    int num_value = cardVal(cardlist, num);
    Card *num_card = findCard(cardlist, num_value);
    num_card = num_card->next;

    // if we need to move 27 cards
    if (num == 27)
    {
        // do nothing.
        return cardlist;
    }
    // else
    // move num top cards above bottom-most card
    bef_bot->next = cardlist;
    bot_card->next = num_card->next;
    num_card->next = bot_card;
    cardlist = bot_card->next;
    return cardlist; // remember to make sure cardlist points to the top card
}

// Free all memory for all cards in
// the circular linked list of cards.
void deallocateCards (Card *cardlist)
{
    // if cardlist is empty, do nothing.
    if (cardlist == NULL) return;
    Card *prev;
    Card *curr = cardlist;
    // if cardlist has next.. 
    if(cardlist->next)
    {   curr = curr->next; // make curr point to cardlist->next
    	while(curr->next != cardlist) 
    	{
        	prev = curr;
        	curr = curr->next;
        	free(prev); // might free nothing sometimes
    	}
    	free(curr->next); // free cardlist
	free (curr);  // free current.
    }
    else { free (curr); } // else just free cardlist. 
}
