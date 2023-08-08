/*
Author: Luke Marshall
Description: Functions to be used in Blackjack.c to play an interactive Blackjack game
*/

#include "Blackjack.h"


/***********************************************
 Card Functions
***********************************************/

//Initializes a card with a given suit and rank
Card *get_card(Suit suit, Rank rank, int value){
    Card *new_card = malloc(sizeof(Card));
    new_card->suit = suit;
    new_card->rank = rank;
    new_card->value = value;
    return new_card;
    
}

//Returns the string equivalent of the Suit enum for use in print statements
char *get_suit(Suit suit){
    //ATTN: returns a pointer to allocated memory, must be freed in calling function!
    char *returned_suit = malloc(10 * sizeof(char));

    switch (suit)
    {
    case Spades:
        snprintf(returned_suit, sizeof(returned_suit) + 1, "Spades");
        break;
    case Hearts:
        snprintf(returned_suit, sizeof(returned_suit) + 1, "Hearts");
        break;
    case Diamonds:
        snprintf(returned_suit, sizeof(returned_suit) + 1, "Diamonds");
        break;
    case Clubs:
        snprintf(returned_suit, sizeof(returned_suit) + 1, "Clubs");
        break;
    default:
        snprintf(returned_suit, sizeof(returned_suit) + 1, "error");
        break;
    }
    return returned_suit;
}

//Returns the string equivalent of the Rank enum for use in print statements
char *get_rank(Rank rank){
    //ATTN: returns a pointer to allocated memory, must be freed in calling function!
    char *returned_rank = malloc(10 * sizeof(char));

    switch (rank){
    case Ace:
        snprintf(returned_rank, sizeof(returned_rank), "Ace");
        break;
    case Two:
        snprintf(returned_rank, sizeof(returned_rank), "Two");
        break;
    case Three:
        snprintf(returned_rank, sizeof(returned_rank), "Three");
        break;
    case Four:
        snprintf(returned_rank, sizeof(returned_rank), "Four");
        break;
    case Five:
        snprintf(returned_rank, sizeof(returned_rank), "Five");
        break;
    case Six:
        snprintf(returned_rank, sizeof(returned_rank), "Six");
        break;
    case Seven:
        snprintf(returned_rank, sizeof(returned_rank), "Seven");
        break;
    case Eight:
        snprintf(returned_rank, sizeof(returned_rank), "Eight");
        break;
    case Nine:
        snprintf(returned_rank, sizeof(returned_rank), "Nine");
        break;
    case Ten:
        snprintf(returned_rank, sizeof(returned_rank), "Ten");
        break;
    case Jack:
        snprintf(returned_rank, sizeof(returned_rank), "Jack");
        break;
    case Queen:
        snprintf(returned_rank, sizeof(returned_rank), "Queen");
        break;
    case King:
        snprintf(returned_rank, sizeof(returned_rank), "King");
        break;
    default:
        snprintf(returned_rank, sizeof(returned_rank), "error");
        break;
    }
    return returned_rank;
}


/***********************************************
 Deck Functions
***********************************************/

//Initializes the number of decks given as input, each deck has each card of each suit and rank
Deck *get_decks(long num_decks){
    if (num_decks < 1 || num_decks > 8){
        exit(0);
    }
    Deck *new_deck = malloc(sizeof(Deck));
    new_deck->cards = malloc(num_decks * DECK_SIZE * sizeof(Card *));
    new_deck->curr = 0;
    new_deck->num_cards = num_decks * DECK_SIZE;

    int index = 0;
        for (int j = 0; j < num_decks; j++){
            for (int k = 1; k < (NUM_RANKS + 1); k++){
                for (int l = 0; l < NUM_SUITS; l++){
                    if (k > 9){
                        new_deck->cards[index] = get_card((Suit) l, (Rank) k, 10);
                    } else if (k == 1){
                        new_deck->cards[index] = get_card((Suit) l, (Rank) k, 11);
                    } else {
                        new_deck->cards[index] = get_card((Suit) l, (Rank) k, k);
                    }
                    index++;
                }
            }
        } 
    return new_deck;
}

//Shuffles the deck once it has been initialized 
void shuffle(Deck *deck, long num_decks){
    Card *tmp = NULL;
    int random;
    for (int j = 0; j < 4; j++){
        for (int i = 0; i < (num_decks * DECK_SIZE); i++){
            random = rand() % (num_decks * DECK_SIZE);
            tmp = deck->cards[random];
            deck->cards[random] = deck->cards[i];
            deck->cards[i] = tmp;
        }
    }
}

//Returns a pointer to the top card of the deck to be added to a players hand and moves the top to the next index position
Card *pop(Deck *deck){
    Card *card = deck->cards[deck->curr];
    deck->curr++;
    return card;
}

//Frees the memory associated with the deck
void free_deck(Deck *deck, long num_decks){
    for (int i = 0; i < num_decks * DECK_SIZE; i++){
        free(deck->cards[i]);
    }
    free(deck->cards);
    free(deck);
}


/***********************************************
 Player Functions
***********************************************/

//Initializes a player that has a hand of cards, a bool representing if they are still playing, a bet for the round, and
//a bank with the amount of money they have left
void get_player(Player *player){
    player->hand = malloc(15 * sizeof(Card *));
    player->total = 0;
    player->curr = 0;
    player->playing = true;
    player->bet = 0.;
    player->bank = STARTING_BANK;
}

//Creates a player array to play the game
Player *get_all_players(long num_players){
    Player *player_array = malloc(num_players * sizeof(Player));
    for (int i = 0; i < num_players; i++){
        get_player(&(player_array[i]));
    }
    return player_array;
}

//Places a bet amount into a Player
int wager(Player *player, double wager){
    player->bet = wager;
    return 1;
}

//Calls pop() to add a card to a player's hand and calculates the new total
int player_hit(Deck *deck, Player *player){
    if (deck->num_cards == deck->curr){
        printf("The dealer has run out of cards.\nExiting Blackjack. Thanks for playing!\n");
        exit(0);
    }
    Card *new_card = pop(deck);
    player->hand[player->curr] = new_card;
    player->total += player->hand[player->curr]->value;
    player->curr++;
    char *suit = get_suit(new_card->suit);
    char *rank = get_rank(new_card->rank);
    printf("Player added a %s of %s to their hand, total is now %d.\n", rank, suit, player->total);
    free(suit);
    free(rank);
    if (player_bust_check(player)){
        return 0;
    }
    if (player_natural_check(player)){
        return -1;
    }
    return 1;
}

//Checks if a given player has achieved a natural (21)
int player_natural_check(Player *player){
    if (player->total == 21){
        printf("Player has achieved 21! Wait for dealer end total to find out the payout.\n");
        return 1;
    } else {
        return 0;
    }
}

//In the case that a player gets to 21 in three cards and the dealer doesn't
void player_natural(Player *player){
    player->bank += (1.5 * player->bet);
    player->bet = 0;
    player->playing = false;
}

//Check if a player has gone bust, also checking if they have any aces that may be revalued to decrease 
int player_bust_check(Player *player){
    if (player->total > 21){
        for (int i = 0; player->hand[i] != NULL; i++){
            if ((player->hand[i]->rank == Ace) && (player->hand[i]->value == 11)) {
                printf("Player Ace value changed to 1 to try to avoid going bust.\nPlayer total is now %d.\n", (player->total - 10));
                player->hand[i]->value = 1;
                player->total -= 10;
                if (player->total < 21){
                    return 0;
                } else if (player_natural_check(player)) {
                    player->playing = false;
                    return 0;
                }
            }
        }
        printf("Player went bust, end of round for player.\n");
        player->playing = false;
        return 1;
    } else {
        return 0;
    }
}

//In the case that a player doesnt get over the dealer's score or exceeds a score of 21
void player_bust(Player *player){
    player->bank -= player->bet;
    player->bet = 0;
    player->playing = false;
}

//In the case that the dealer and player both get a natural or the dealer runs out of cards
void offset(Player *player){
    player->bet = 0.;
    player->playing = false;
}

//Resest each player to playing when a round is over. 
void player_new_round(Player *player){
    if (player->bank){
        player->playing = true;
        player->bet = 0.;
        for (int i = 0; player->hand[i] != NULL; i++){
            player->hand[i] = NULL;
        }
        player->total = 0;
        player->curr = 0;
    }
}

//Prints the cards a player has in their hand and the total they amount to
void player_print_hand(Player *player){
    printf("Your hand is:\n");
    for (int i = 0; i < (player->curr); i++){
        char *suit = get_suit(player->hand[i]->suit);
        char *rank = get_rank(player->hand[i]->rank);
        printf("%s of %s\n", rank, suit);
        free(suit);
        free(rank);
    }
    printf("Your card total is %d\n", player->total);
}

//Prints the amount of money a given player has left
void print_bank(Player *player){
    printf("You have $%.2f left to play.\n", player->bank);
}

void player_total(Player *player){
    printf("Your total is: %d\n", player->total);
}

//Frees the memory associated with a player
void free_players(Player *players, int num_players){
    for (int i = 0; i < num_players; i++){
        free(players[i].hand);
    }
    free(players);
}


/***********************************************
 Dealer Functions
***********************************************/

//Creates a dealer 
Dealer *get_dealer(){
    Dealer *dealer = malloc(sizeof(Dealer));
    dealer->hand = malloc(15 * sizeof(Card *));
    dealer->curr = 0;
    dealer->total = 0;
    return dealer;
}

//Checks if the dealer has a natural
int dealer_natural_check(Dealer *dealer){
    return (dealer->total == 21);
}

//Makes appropriate transactions on a player's bank given their bet and total
/*int dealer_natural(Player *player){
    if (player->total != BLACKJACK){
        player->bank -= player->bet;
        player_new_round(player);
        return 1;
    } else {
        offset(player);
        player_new_round(player);
        return 0;
    }
}*/


//Checks if the dealer has went bust
int dealer_bust_check(Dealer *dealer){
    if (dealer->total > 21){
        return dealer_ace_behavior(dealer);
    } else {
        return 0;
    }
}

//If dealer goes bust, check for ace and revalue it
int dealer_ace_behavior(Dealer *dealer){
    for (int i = 0; (dealer->hand[i]) != NULL; i++)
        if ((dealer->hand[i]->rank == Ace) && (dealer->hand[i]->value == 11)){
            dealer->hand[i]->value = 1;
            dealer->total -= 10;
            printf("The dealer has revalued their Ace to 1 point to avoid going bust, their total is now %d.\n", dealer->total);
            if (dealer->total <= 21){
                return 0;
            }    
        }
    return 1;
}

//If the dealer goes bust, performs the required transactions with players bets based on their totals
void dealer_bust(Dealer *dealer, Player *players, long num_players){
    for (int i = 0; i < num_players; i++){ 
        if (players[i].bank == -42.){
            continue;
        }
        if (players[i].total == BLACKJACK){
            printf("Player %d received $%.2f for reaching Blackjack.\n", (i+1), (1.5 * players[i].bet));
            player_natural(&players[i]);
        } else if (players[i].total < BLACKJACK){
            printf("Player %d received $%.2f for having a lower total than the 21.\n", (i+1), players[i].bet);
            players[i].bank += players[i].bet;
        } else if (players[i].total > BLACKJACK) {
            printf("Player %d lost $%.2f for going bust.\n", (i+1), players[i].bet);
            player_bust(&players[i]);
        } 
        player_new_round(&players[i]);
    }
    dealer_new_round(dealer);
}

//If every player stands or went bust and the dealer stands at less than 21
void end_round(Dealer *dealer, Player *players, long num_players){
    for (int i = 0; i < num_players; i++){
        if (players[i].bank == -42.){
            continue;
        }
        if ((players[i].total == BLACKJACK) && (dealer->total != BLACKJACK)){
            printf("Player %d received $%.2f for reaching Blackjack.\n", (i+1), (1.5 * players[i].bet));
            player_natural(&players[i]);
        } else if (players[i].total == dealer->total){
            printf("Player %d had the same total as the dealer, no amount is exchanged.\n", (i+1));
            offset(&players[i]);
        } else if (players[i].total < dealer->total){
            printf("Player %d lost $%.2f for having a lower total than the dealer.\n", (i+1), players[i].bet);
            players[i].bank -= players[i].bet;
        } else if (players[i].total > BLACKJACK) { 
            printf("Player %d lost $%.2f for going bust.\n", (i+1), players[i].bet);
            player_bust(&players[i]);
        } else {
            printf("Player %d received $%.2f for having a higher total than the dealer.\n", (i+1), players[i].bet);
            players[i].bank += players[i].bet;
        }
        player_new_round(&players[i]);
    }
    dealer_new_round(dealer);
}

//Saves the current top card to be placed in the dealer's hand at the end of all the player's turns; 
//simulates the drawing of a card face down by the dealer 
Card *saved_card(Deck *deck){
    if (deck->num_cards == deck->curr){
       printf("The dealer has run out of cards.\nExiting Blackjack. Thanks for playing!\n");
       exit(0);
    }
    printf("The dealer has taken a card face-down\n");
    return pop(deck);
}

//Checks the dealer's total; if < 17, dealer hits
int dealer_hit_check(Dealer *dealer){
    return (dealer->total < 17);
}

//Uses the saved card and puts it in the dealer's hand
//The saving and then adding of the card simulates the taking of a face-down card by the dealer at the start of the round and then flipping it at their turn
int dealer_hit_saved(Dealer *dealer, Card *down_card){
    dealer->hand[dealer->curr] = down_card;
    dealer->total += down_card->value;
    dealer->curr++;
    char *card_suit = get_suit(down_card->suit);
    char *card_rank = get_rank(down_card->rank);
    printf("Dealer added the %s of %s to their hand from their face-down card, their total is now %d.\n", card_rank, card_suit, dealer->total);
    free(card_suit);
    free(card_rank);
    if (dealer_bust_check(dealer)){
        printf("Dealer went bust, the round is over and appropriate transactions will occur.\n");
        return 0;
    }
    if (dealer_natural_check(dealer)){
        printf("Dealer's total is 21, the round is over and appropriate transactions will occur.\n");
        return -1;
    }
    return 1;
}

//hit function for after all plaeyrs are standing
int dealer_hit(Dealer *dealer, Deck *deck){
    if (deck->num_cards == deck->curr){
       printf("The dealer has run out of cards.\nExiting Blackjack. Thanks for playing!\n");
       exit(0);
    }
    Card *new_card = pop(deck);
    dealer->hand[dealer->curr] = new_card;
    dealer->total += new_card->value;
    dealer->curr++;
    char *card_suit = get_suit(new_card->suit);
    char *card_rank = get_rank(new_card->rank);
    printf("Dealer added the %s of %s to their hand, their total is now %d.\n", card_rank, card_suit, dealer->total);
    free(card_suit);
    free(card_rank);
    if (dealer_bust_check(dealer)){
        printf("Dealer went bust, the round is over and winnings will be distributed.\n");
        return 0;
    }
    if (dealer_natural_check(dealer)){
        printf("Dealer's total is 21, the round is over and proper transactions will occur.\n");
        return -1;
    }
    return 1;

}

//Resets the dealer between hands
void dealer_new_round(Dealer *dealer){
    for (int i = 0; dealer->hand[i] != NULL; i++){
        dealer->hand[i] = NULL;
    }
    dealer->total = 0;
    dealer->curr = 0;
}

//Prints the cards a player has in their hand and the total they amount to
void dealer_print_hand(Dealer *dealer){
    if (dealer->curr == 0){
        printf("The dealer holds no cards or has not flipped their first card yet.\n");
    }
    printf("Dealer's hand is:\n");
    for (int i = 0; i < (dealer->curr); i++){
        char *suit = get_suit(dealer->hand[i]->suit);
        char *rank = get_rank(dealer->hand[i]->rank);
        printf("%s of %s\n", rank, suit);
        free(suit);
        free(rank);
    }
    printf("Dealer's card total is %d.\n", dealer->total);
}

//Frees memory associated with the dealer
void free_dealer(Dealer *dealer){
    free(dealer->hand);
    free(dealer);
}