#include "player.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int add_card(player *asker, player *askee, char rank) {
    int count = 0;
    card *transferred_cards = NULL;
    
    printf("    - %s has ", askee->name);
    
    // First pass: count matching cards
    for (int i = 0; i < askee->hand_size; i++) {
        if (askee->hand[i].rank == rank) {
            count++;
        }
    }
    
    // Allocate memory for transferred cards
    if (count > 0) {
        transferred_cards = (card *)malloc(count * sizeof(card));
        if (transferred_cards == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
    
    // Second pass: transfer matching cards
    int transfer_index = 0;
    for (int i = 0; i < askee->hand_size; i++) {
        if (askee->hand[i].rank == rank) {
            printf("%c%c ", askee->hand[i].rank, askee->hand[i].suit);
            transferred_cards[transfer_index++] = askee->hand[i];
            
            // Remove card from askee's hand
            for (int j = i; j < askee->hand_size - 1; j++) {
                askee->hand[j] = askee->hand[j + 1];
            }
            askee->hand_size--;
            i--; // Adjust index since we removed a card
        }
    }
    
    // Add transferred cards to asker's hand
    for (int i = 0; i < count; i++) {
        asker->hand[asker->hand_size++] = transferred_cards[i];
    }
    
    if (count == 0) {
        printf("no %c's", rank);
    }
    printf("\n");
    
    // Free allocated memory
    free(transferred_cards);
    
    return count;
}

int check_add_book(player *p) {
  char ranks[MAX_RANKS] = "23456789TJQKA";
  char suits[MAX_SUITS] = "CDHS";

  for (int i = 0; i < MAX_RANKS; i++) {
    int count = 0;
    char rank = ranks[i];
    char collected_suits[MAX_SUITS];
    int suit_count = 0;

    for (int j = 0; j < p->hand_size; j++) {
      if (p->hand[j].rank == rank) {
        collected_suits[suit_count++] = p->hand[j].suit;
        count++;
      }
    }

    if (count == 4) {
      p->books[i] = 1;
      p->book_count++;

      printf("    - %s books %c \n", p->name, rank);

      for (int k = 0; k < p->hand_size; k++) {
        if (p->hand[k].rank == rank) {
          for (int l = k; l < p->hand_size - 1; l++) {
            p->hand[l] = p->hand[l + 1];
          }
          p->hand_size--;
          k--;
        }
      }
      return 1;
    }
  }
  return 0;
}

void draw_card_check(card deck[], int *deck_index, player *p) {
  if (*deck_index < MAX_CARDS) {
    p->hand[p->hand_size++] = deck[(*deck_index)++];
    if (strcmp(p->name, "Player 1") == 0) {
      printf("    - %s draws %c%c\n", p->name, p->hand[p->hand_size - 1].rank,
             p->hand[p->hand_size - 1].suit);
    } else {
      printf("    - %s draws a card\n", p->name);
    }
  } else {
    printf("    - The deck is empty. No card drawn.\n");
  }
}

int game_over(player players[], int num_players) {
    for (int i = 0; i < num_players; i++) {
        if (players[i].book_count >= BOOKS_TO_WIN) {
            return 1;
        }
    }
    return 0;
}

int search(player *p, char rank) {
    for (int i = 0; i < p->hand_size; i++) {
        if (p->hand[i].rank == rank) {
            return 1;
        }
    }
    return 0;
}

void reset_player(player players[], int num_players) {
    for (int i = 0; i < num_players; i++) {
        players[i].hand_size = 0;
        players[i].book_count = 0;
        memset(players[i].books, 0, sizeof(players[i].books));
    }
}

char computer_play(player *p) {
    if (p->hand_size == 0) {
        return '\0';
    }
    int random_index = rand() % p->hand_size;
    return p->hand[random_index].rank;
}



void user_play(card deck[], player players[], int num_players) {
    int deck_index = num_players * MAX_HAND_SIZE;
    int player_num = 0;
    char rank;

    while (!game_over(players, num_players)) {
        if (player_num == 0) { // If it's Player 1's turn
            printf("%s's Hand - ", players[player_num].name);
            print_hand(&players[player_num]); // Print Player 1's hand
            print_books(&players[0]);
            print_books(&players[1]);
        } else { // If it's Player 2's turn
            printf("%s's Hand - ", players[0].name);
            print_hand(&players[0]);
            print_books(&players[0]);
            print_books(&players[1]);
            printf("\n"); // Print "blank" for Player 2
        }

        int turn_over = 0;
        while (!turn_over && players[player_num].hand_size > 0) {
            if (player_num == 0) {
                printf("%s's turn, enter a Rank: ", players[player_num].name);
                scanf(" %c", &rank);
                rank = toupper(rank);

                if (!search(&players[player_num], rank)) {
                    printf("Error - must have at least one card from rank to play\n");
                    continue;
                }
            } else {
                rank = computer_play(&players[player_num]);
                printf("%s's turn, asks for Rank: %c\n", players[player_num].name, rank);
            }

            int askee = (player_num + 1) % num_players;
            int cards_transferred = add_card(&players[player_num], &players[askee], rank);
            
            if (cards_transferred > 0) {
                // printf("", players[askee].name, cards_transferred, rank);
                check_add_book(&players[player_num]);
            } else {
                // printf("    - %s has no %c's\n", players[askee].name, rank);
                draw_card_check(deck, &deck_index, &players[player_num]);
                if (players[player_num].hand[players[player_num].hand_size - 1].rank == rank) {
                    // printf("    - %s drew the card they asked for (%c)\n", players[player_num].name, rank);
                    check_add_book(&players[player_num]);
                } else {
                    turn_over = 1;
                }
            }

            if (players[player_num].hand_size == 0 && deck_index < MAX_CARDS) {
                draw_card_check(deck, &deck_index, &players[player_num]);
            }

            if (game_over(players, num_players)) {
                break;
            }
        }

        if (game_over(players, num_players)) {
            break;
        }

        player_num = (player_num + 1) % num_players;
        printf("\n");
    }

    printf("\nGame Over!\n");
    for (int i = 0; i < num_players; i++) {
        print_books(&players[i]);
    }

    if (players[0].book_count > players[1].book_count) {
        printf("%s Wins! %d - %d\n", players[0].name, players[0].book_count, players[1].book_count);
    } else if (players[1].book_count > players[0].book_count) {
        printf("%s Wins! %d - %d\n", players[1].name, players[0].book_count, players[1].book_count);
    } else {
        printf("It's a tie!");
    }
}

void print_hand(player *p) {
  for (int i = 0; i < p->hand_size; i++) {
    printf("%c%c ", p->hand[i].rank, p->hand[i].suit);
  }
  printf("\n");
}

void print_books(player *p) {
  printf("%s's Book - ", p->name);
  char ranks[MAX_RANKS] = "23456789TJQKA";
  int printed = 0;
  for (int i = 0; i < MAX_BOOKS; i++) {
    if (p->books[i]) {
      printf("%c ", ranks[i]);
      printed = 1;
    }
  }
  if (!printed) {
    printf("");
  }
  printf("\n");
}
