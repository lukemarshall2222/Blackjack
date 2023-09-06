#ifndef BLACKJACK_HEADER
#define BLACKJACK_HEADER

#define DECK_SIZE 52
#define NUM_SUITS 4
#define NUM_RANKS 13
#define STARTING_BANK 100.
#define BLACKJACK 21

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

typedef enum {
    Spades = 0,
    Hearts,
    Diamonds,
    Clubs
} Suit;

typedef enum {
    Ace = 1,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
} Rank;

typedef struct {
    Suit suit;
    Rank rank;
    int value;
} Card;

typedef struct {
    Card **cards;
    int curr;
    int num_cards;
} Deck;

typedef struct {
    Card **hand;
    int total;
    int curr;
    bool playing;
    double bet;
    double bank;
} Player;

typedef struct {
    Card **hand;
    int total;
    int curr;
} Dealer;

//card functions
Card *get_card(Suit suit, Rank rank, int value);
char *get_suit(Suit suit);
char *get_rank(Rank rank);

//deck functions
Deck *get_decks(long num_decks);
void shuffle(Deck *deck, long num_decks);
Card *pop(Deck *deck);
int empty_deck(Deck *deck);
void free_deck(Deck *deck, long num_decks);

//player functions
void get_player(Player *player);
Player *get_all_players(long num_players);
int wager(Player *player, double wager);
int player_hit(Deck *deck, Player *player);
int player_natural_check(Player *player);
void player_natural(Player *player);
int player_bust_check(Player *player);
void player_bust(Player *player);
void offset(Player *player);
void player_new_round(Player *player);
void player_print_hand(Player *player);
void print_bank(Player *player);
void player_total(Player *player);
void free_players(Player *players, int num_players);

//Dealer Functions
Dealer *get_dealer();
int dealer_natural_check(Dealer *dealer);
//int dealer_natural(Player *player);
int dealer_bust_check(Dealer *dealer);
int dealer_ace_behavior(Dealer *dealer);
void dealer_bust(Dealer *dealer, Player *players, long num_players);
void end_round(Dealer *dealer, Player *players, long num_players);
Card *saved_card(Deck *deck);
int dealer_hit_check(Dealer *dealer);
int dealer_hit_saved(Dealer *dealer, Card *down_card);
int dealer_hit(Dealer *dealer, Deck *deck);
void dealer_new_round(Dealer *dealer);
void dealer_print_hand(Dealer *dealer);
void free_dealer(Dealer *dealer);

void exit_game(Dealer *dealer, Player *players, long num_players, Deck *deck);

#endif