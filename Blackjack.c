/*
Author: Luke Marshall
Description: A simplified blackjack game. 
Simplifications include: no splitting a hand, and aces default to 11 until the holder would bust then are revalued to 1 automatically
*/

#include "Blackjack.h"

int main(){
    //Initialize the decks
    long num_decks;
    char char_num_decks[10];
    char *d_end;
    while (1){
        printf("Please enter a whole number at least 1 and a maximum of 8 for the number of decks that will be used (leave empty to exit): ");
        char *d_checker = fgets(char_num_decks, sizeof(char_num_decks), stdin);
        if (char_num_decks[0] == '\n'){
            printf("Exiting Blackjack. Thanks for playing!\n");
            exit(0);
        }
        if (char_num_decks[strlen(char_num_decks) - 1] == '\n'){
            char_num_decks[strlen(char_num_decks) - 1] = '\0';
        }
        if (d_checker == NULL || !isdigit(char_num_decks[0])){
            fprintf(stderr, "Your input \'%s\' could not be processed, please try again.\n", char_num_decks);
            continue;
        }
        num_decks = strtol(char_num_decks, &d_end, 10);
        if ((num_decks < 1) || (num_decks > 8)){
            fprintf(stderr, "The number %ld is outside the allowed range, please try again.\n", num_decks);
            continue;
        }
        printf("The game will be played with %ld decks.\n", num_decks);
        break;
    }
    printf("Retrieving the decks...");
    Deck *deck = get_decks(num_decks);
    printf("done.\n");
    printf("Shuffling the decks...");
    shuffle(deck, num_decks);
    printf("done.\n");

    //Initialize players
    long num_players;
    char char_num_players[10];
    char *p_end;
    while (1){
        printf("Please enter a whole number at least 1 and a maximum of 8 for the number of players that will be playing (leave empty to exit): ");
        char *p_checker = fgets(char_num_players, sizeof(char_num_players), stdin);
        if (char_num_players[0] == '\n'){
            printf("Exiting Blackjack. Thanks for playing!\n");
            exit(0);
        }
        if (char_num_players[strlen(char_num_players) - 1] == '\n'){
            char_num_players[strlen(char_num_players) - 1] = '\0';
        }
        if (p_checker == NULL || !isdigit(char_num_players[0])){
            fprintf(stderr, "Your input \'%s\' could not be processed, please try again.\n", char_num_players);
            continue;
        }
        num_players = strtol(char_num_players, &p_end, 10);
        if ((num_players < 1) || (num_players > 8)){
            fprintf(stderr, "The number %ld is outside the allowed range, please try again.\n", num_players);
            continue;
        }
        printf("The game will be played with %ld players.\n", num_players);
        break;
    }
    printf("Retrieving players...");
    Player *players = get_all_players(num_players);
    printf("done.\nThe game may begin!\n");

    //Initialize dealer
    Dealer *dealer = get_dealer();

    int round_num = 1;
    int player_check = num_players;
    while (player_check > 0){
        player_check = num_players;
        printf("Round %d is beginning.\n", round_num);
        //Collect bets from players
        int i = 0;
        long bet;
        char char_bet[10];
        char *b_end;
        while (i < num_players) {
            //player availability checks
            if (players[i].bank == -42.){
                printf("Player %d left the game, their turn will be skipped.\n", (i+1));
                players[i].playing = false;
            } else if (players[i].bank < 1.){
                printf("Player %d has insufficient funds, their turn will be skipped.\n", i);
                players[i].playing = false;
            }
            if (!(players[i].playing)){
                i++;
                player_check--;
                if (player_check < 1){
                printf("All players have either left the game or have insufficient funds to continue playing.\nExiting Blackjack. Thanks for playing!\n");
                goto Exit;
                }
                continue;
            }
            

            bet = 0;
            printf("Player %d, enter your bet (whole number, minimum 1, maximum is the amount in your bank, enter 'b' to see what is in your bank, leave blank to leave the game.): ", (i+1));
            char *b_checker = fgets(char_bet, sizeof(char_bet), stdin);
            if (char_bet[0] == '\n'){
                printf("Thanks for playing! Player %d is now leaving the game with $%.2f.\n", (i+1), players[i].bank);
                players[i].bank = -42.;
                continue;
            }
            if (char_bet[strlen(char_bet) - 1] == '\n'){
                char_bet[strlen(char_bet) - 1] = '\0';
            }
            if (char_bet[0] == 'b'){
                print_bank(&(players[i]));
                continue;
            }
            if (b_checker == NULL || !isdigit(char_bet[0])){
                fprintf(stderr, "Your input \'%s\' could not be procesed, please try again.\n", char_bet);
                continue;
            }
            bet = strtol(char_bet, &b_end, 10);
            if (bet < 1){
                fprintf(stderr, "The amount $%ld is less than 1, please try again.\n", bet);
                continue;
            }
            if (bet > players[i].bank){
                fprintf(stderr, "The amount $%ld is more than you have in your bank, please try again.\n", bet);
                continue;
            }
            wager(&(players[i]), bet);
            printf("Your bet of $%ld has been placed.\n", bet);
            i++;
        }


        //Distribute two cards per player and dealer
        if (empty_deck(deck)){
            goto Exit;
        }
        int player_num = 0;
        Card *down_card = saved_card(deck);
        for (int j = 0; j < 2; j++){
            for (int i = 0; i < num_players; i++){
                if (empty_deck(deck)) {
                    goto Exit;
                }
                if (players[i].playing){
                    player_num = (i % num_players) + 1;
                    printf("Player %d:\n", player_num);
                    player_hit(deck, &(players[i]));
                }
            }
            if (j == 0){
                    dealer_hit_saved(dealer, down_card);
                    down_card = saved_card(deck);
                }
            }
            

        //Play a round based on user input
        int h_status;
        char hit_stay[5];
        bool continuation;
        for (int i = 0; i < num_players; i++){
            continuation = true;
            if (players[i].playing){
                while (continuation){
                    if (!players[i].playing){
                        continuation = false;
                        continue;
                    }
                    
                    if (empty_deck(deck)) {
                        goto Exit;
                    }

                    printf("Player %d, please type and enter 'h' to hit and 's' to stay.(Enter 'p' to see your hand,'d' to see the dealer's face-up card, 'b' to see what is in your bank, or 't' to see your total): ", (i+1));
                    char *h_checker = fgets(hit_stay, sizeof(hit_stay), stdin);

                    if (hit_stay[strlen(hit_stay) - 1] == '\n'){
                        hit_stay[strlen(hit_stay) - 1] = '\0';
                    }
                    if ((strlen(h_checker) == 0) || h_checker == NULL){
                        fprintf(stderr, "Your input \'%s\' could not be processed, please try again.\n", hit_stay);
                    }

                    switch (hit_stay[0]){
                        case 's':
                            printf("Player stays with a total of %d.\n", players[i].total);
                            continuation = false;
                            break;
                        case 'p':
                            player_print_hand(&players[i]);
                            break;
                        case 'b':
                            print_bank(&players[i]);
                            break;
                        case 't':
                            player_total(&players[i]);
                            break;
                        case 'h':
                            h_status = player_hit(deck, &players[i]);
                            if ((h_status == 0) || (h_status == -1)){
                                continuation = false;
                            }
                            break;
                        case 'd':
                            dealer_print_hand(dealer);
                            break;
                        default:
                            fprintf(stderr, "Your input '%s' could not be processed, please try again.\n", hit_stay);
                            break;
                    }
                }
            }
        }
        int d_status = dealer_hit_saved(dealer, down_card);
        while (dealer_hit_check(dealer)){
            d_status = dealer_hit(dealer, deck);
        }

        if (d_status == 0){
            dealer_bust(dealer, players, num_players);
        } else {
            if (d_status == 1){
                printf("Everyone stands, the round is over, appropriate transactions will occur.\n");
            }
            end_round(dealer, players, num_players);
        }
        round_num++;
    }

    Exit:
    exit_game(dealer, players, num_players, deck);
    return 0;
}