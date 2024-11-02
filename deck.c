#include "deck.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Structure: deck
 * ---------------
 *  An array of 52 cards.
 */

struct deck deck_instance;


/*
 * Function: shuffle
 * ----------------------
 *
 * This function shuffles the deck of cards
 */
void shuffle(card deck[]) {
  for (int i = 0; i < MAX_CARDS; i++) {
    int j = rand() % MAX_CARDS;
    card temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
  }
}

/*
 * Function: deal_player_cards
 * ---------------------------
 *  Deal 7 random cards to the player specified in the function.
 *  Remove the dealt cards from the deck. 
 *
 *  target: pointer to the player to be dealt cards
 *
 *  returns: 0 if no error, and non-zero on error
 */

void deal_player_cards(card deck[], player players[], int num_players) {
  int index = 0;
  for (int i = 0; i < MAX_HAND_SIZE; i++) {
    for (int j = 0; j < num_players; j++) {
      players[j].hand[i] = deck[index];
      players[j].hand_size++;
      index++;
    }
  }
}
/*
 * Function: size
 * --------------
 *  Return the number of cards left in the current deck.
 *
 *  returns: number of cards left in the deck.
 */
struct card *next_card() {
  deck_instance.top_card--;
  return &deck_instance.cards[deck_instance.top_card + 1];
}

/*
 * Function: deck_size()
 * --------------------
 *
 * This function prints the hand of cards
 */
size_t deck_size() { return deck_instance.top_card + 1; }

void initialize_deck(card deck[]) {
  char ranks[MAX_RANKS] = "23456789TJQKA";
  char suits[MAX_SUITS] = "CDHS";
  int index = 0;
  for (int i = 0; i < MAX_RANKS; i++) {
    for (int j = 0; j < MAX_SUITS; j++) {
      deck[index].rank = ranks[i];
      deck[index].suit = suits[j];
      index++;
    }
  }
}
