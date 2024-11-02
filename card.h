#ifndef CARD_H
#define CARD_H

#define MAX_CARDS 52
#define MAX_RANKS 13
#define MAX_SUITS 4
#define MAX_BOOKS 13
#define BOOKS_TO_WIN 7

/*
  Valid suits: C, D, H, and S
  Valid ranks: 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A
*/
typedef struct card {
    char rank;
    char suit;
} card;

/*
  Linked list of cards in hand.
    top: first card in hand
    next: pointer to next card in hand
*/
struct hand
{
  struct card top;
  struct hand* next;
};

#endif
