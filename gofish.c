#include "deck.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int main() {
    srand(time(NULL));
    int play_again = 1;
    while (play_again) {
        printf("\n");
        printf("Shuffling deck...\n");
        printf("\n");
        card deck[MAX_CARDS];
        initialize_deck(deck);
        shuffle(deck);
        int num_players = 2;
        player players[num_players];
        strcpy(players[0].name, "Player 1");
        strcpy(players[1].name, "Player 2");
        reset_player(players, num_players);
        deal_player_cards(deck, players, num_players);
        user_play(deck, players, num_players); 
        char choice;
        printf("Do you want to play again [Y/N]: ");
        scanf(" %c", &choice);
        play_again = (toupper(choice) == 'Y');
    }
    printf("Exiting.\n");
    return 0;
}