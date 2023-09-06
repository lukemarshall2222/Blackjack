/*
Author: Luke Marshall
Description: Tests for Blackjack code
*/

#include "Blackjack.h"

//Used for comparing cards in the test functions
int compare_cards(Card *first, Card *second){
    if (first->rank != second->rank){
        //fprintf(stderr, "The card ranks do not match.\n");
        return 0;
    } else if (first->suit != second->suit){
        //fprintf(stderr, "The card suits do not match.\n");
        return 0;
    } else if (first->value != second->value){
        //fprintf(stderr, "The card values do not match.\n");
        return 0;
    } else {
        return 1;
    }
}

//Test for the get_card function
int test_get_card(Suit suit, Rank rank, int value){

    //create the cards using function
    Card *first_card = get_card(Clubs, Ace, 11);
    Card *second_card = get_card((Suit) 3, (Rank) 1, 11);
    Card *third_card = get_card(suit, rank, value);

    //Create card to compare with function outputs
    Card compare_card;
    compare_card.rank = Ace;
    compare_card.suit = Clubs;
    compare_card.value = 11;

    //Test the outputs against the known card
    bool failed = false;
    if (!compare_cards(first_card, &compare_card)){
        fprintf(stderr, "First card does not match expcted output.\n");
        failed = true;
    }
    if (!compare_cards(second_card, &compare_card)){
        fprintf(stderr,"Second card does not match expected output.\n");
        failed = true;
    }
    if (!compare_cards(third_card, &compare_card)){
        fprintf(stderr,"Third card does not match expected output.\n");
        failed = true;
        }
    if (failed){
        return 0;
    } else {
        return 1;
    }
}

//Test for the get_decks function
int test_get_decks(long num_decks){
    bool failed = false;

    //create deck
    Deck *deck = get_decks(num_decks);

    //count the cards in the deck
    int count = 0;
    for (int i = 0; deck->cards[i] != NULL; i++){
        count++;
    }

    //Check if the number of cards in the deck is what it should be
    if (count != (DECK_SIZE * num_decks)){
        fprintf(stderr, "The number of cards in the deck is not correct.");
        failed = true;
    }

    //check the first card in the deck
    Card *first_card = get_card(Spades, Ace, 11);
    if (!compare_cards(first_card, deck->cards[0])){
        fprintf(stderr, "The first card in the deck is not correct. It is the %d of %d, with a value of %d", deck->cards[0]->rank, deck->cards[0]->suit, deck->cards[0]->value);
        failed = true;
    }

    //check the last card in the deck
    Card *last_card = get_card(Clubs, King, 10);
    if (!compare_cards(last_card, deck->cards[DECK_SIZE * num_decks - 1])){
        fprintf(stderr, "The first card in the deck is not correct. It is the %d of %d, with a value of %d", deck->cards[0]->rank, deck->cards[0]->suit, deck->cards[0]->value);
        failed = true;
    }

    if (failed){
        return 0;
    } else {
        return 1;
    }
}

//Test for the shuffle function
int test_shuffle(long num_decks){
    Deck *org_deck = get_decks(num_decks);
    Deck *shuff_deck = get_decks(num_decks);

    shuffle(shuff_deck, num_decks);

    for (int i = 0; i < (DECK_SIZE * num_decks); i++){
        int count = 0;
        if (org_deck->cards[i]->rank == shuff_deck->cards[i]->rank
            && org_deck->cards[i]->suit == shuff_deck->cards[i]->suit
            && org_deck->cards[i]->value == shuff_deck->cards[i]->value){
            count++;
        }
        if (count > (2 * num_decks)){
            fprintf(stderr, "The shuffled deck is not shuffled enough.");
            return 0;
        }

    }
    return 1;
}

//Test for the pop function
int test_pop(long num_decks){
    bool failed = false;

    //Initialize the decks for popping
    Deck *deck_1 = get_decks(num_decks);
    Deck *deck_2 = get_decks(num_decks);
    Deck *deck_3 = get_decks(num_decks);

    //Initialize cards to compare with popped values
    Card *hard_comp_1 = get_card(Spades, Ace, 11);
    Card *hard_comp_2 = get_card(Hearts, Ace, 11);
    Card *hard_comp_3 = get_card(Diamonds, Ace, 11);

    Card *popped = pop(deck_1);
        if (popped->suit != hard_comp_1->suit
            || popped->rank != hard_comp_1->rank
            || popped->value != hard_comp_1->value){
            fprintf(stderr, "The hard coded comparison 1 did not produce the correct results.\n");
        }

    popped = pop(deck_1);
    if (popped->suit != hard_comp_2->suit
        || popped->rank != hard_comp_2->rank
        || popped->value != hard_comp_2->value){
        fprintf(stderr, "The hard coded comparison 2 did not produce the correct results.\n");
        failed = true;
    }

    popped = pop(deck_1);
    if (popped->suit != hard_comp_3->suit
        || popped->rank != hard_comp_3->rank
        || popped->value != hard_comp_3->value){
        fprintf(stderr, "The hard coded comparison 3 did not produce the correct results.\n");
        failed = true;
    }


    //Compare the two unshuffled and unpopped decks card by card
    for (int i = 0; i < (DECK_SIZE * num_decks); i++){
        Card *comp_card_2 = pop(deck_2);
        Card *comp_card_3 = pop(deck_3);
        if (comp_card_2->suit != comp_card_3->suit
            || comp_card_2->rank != comp_card_3->rank
            || comp_card_2->value != comp_card_3->value){
            fprintf(stderr, "The cards popped from index %d are not the same.\n", i);
            failed = true;
        }
    }

    if (failed){
        return 0;
    } else {
        return 1;
    }
}

//test for get_player function
int test_get_player(){

    bool failed = false;

    Player player;
    get_player(&player);

    if (player.hand[0] != NULL){
        fprintf(stderr, "Player hand initiated incorrectly.\n");
        failed = true;
    }
    if (player.total != 0){
        fprintf(stderr, "Player total initialized incorrectly.\n");
        failed = true;
    }
    if (player.curr != 0){
        fprintf(stderr, "Player curr index initialized incorrectly.\n");
        failed = true;
    }
    if (player.playing != true){
        fprintf(stderr, "Player playing initialized incorrectly.\n");
        failed = true;
    }
    if (player.bet != 0){
        fprintf(stderr, "Player bet initialized incrrectly.\n");
        failed = true;
    }
    if (player.bank != STARTING_BANK){
        fprintf(stderr, "Player bank initialized incorrecty.\n");
        failed = true;
    }

    if (failed){
        return 0;
    } else {
        return 1;
    }
}

//test for the bet function
int test_wager(double bet){
    Player player;
    get_player(&player);
    int bet_success = wager(&player, bet);
    if (!bet_success){
        return 0;
    }

    if (player.bet != bet){
        return 0;
    } else {
        return 1;
    }
}

//test for the player_hit function
int test_player_hit(long num_decks){

    bool failed = false;

    Deck *deck = get_decks(num_decks);
    shuffle(deck, num_decks);
    Player player;
    get_player(&player);
    player_hit(deck, &player);
    player_hit(deck, &player);

    if (deck->cards[0] != player.hand[0]){
        fprintf(stderr, "The first hit was not successful.\n");
        failed = true;
    }
    if (deck->cards[1] != player.hand[1]){
        fprintf(stderr, "The second hit was not successful.\n");
        failed = true;
    }
    if (player.total != (deck->cards[0]->value + deck->cards[1]->value)){
        fprintf(stderr, "The total was not calculated correctly for the player.\n");
        failed = true;
    }

    if (failed){
        return 0;
    } else {
        return 1;
    }
}

//test for the natural_check and player_natural functions
int test_natural(){

    bool failed = false;

    //Initialize cards that make 21 with their values
    Card *first_card = get_card(Diamonds, Ace, 11);
    Card *sec_card = get_card(Diamonds, King, 10);

    //Initialize player with the cards and corresponding total
    Player player;
    get_player(&player);
    player.hand[0] = first_card;
    player.hand[1] = sec_card;
    player.total = (first_card->value + sec_card->value);
    double bet = 14.;
    wager(&player, bet);

    //check the player_natural_check function
    if (!player_natural_check(&player)){
        fprintf(stderr, "The player_natural_check function did not perform correctly.\n");
        failed = true;
    } /*else {
        player_natural(&player);
        if (player.bank != (STARTING_BANK + (1.5 * wager))){
            fprintf(stderr, "The player_natural function did not perform correctly.\n");
            failed = true;
        }
    }*/

    if (failed){
        return 0;
    } else {
        return 1;
    } 
}

//test for the player_bust_check and player_bust functions
int test_bust(){
    bool failed = false;

    //Initialize the cards for bust testing
    Card *card_ace = get_card(Hearts, Ace, 11);
    Card *card_8 = get_card(Diamonds, Eight, 8);
    Card *card_5 = get_card(Spades, Five, 5);
    Card *card_king = get_card(Spades, King, 10);

    //Initialize the players for bust testing
    Player non_bust_player;
    get_player(&non_bust_player);
    wager(&non_bust_player, 15);
    non_bust_player.hand[0] = card_5;
    non_bust_player.hand[1] = card_king;
    non_bust_player.total = non_bust_player.hand[0]->value + non_bust_player.hand[1]->value;

    Player player_w_ace;
    get_player(&player_w_ace);
    wager(&player_w_ace, 15);
    player_w_ace.hand[0] = card_5;
    player_w_ace.hand[1] = card_ace;
    player_w_ace.hand[2] = card_8;
    player_w_ace.total = player_w_ace.hand[0]->value + player_w_ace.hand[1]->value + player_w_ace.hand[2]->value;

    Player player_wo_ace;
    get_player(&player_wo_ace);
    wager(&player_wo_ace, 15);
    player_wo_ace.hand[0] = card_5;
    player_wo_ace.hand[1] = card_8;
    player_wo_ace.hand[2] = card_king;
    player_wo_ace.total = player_wo_ace.hand[0]->value + player_wo_ace.hand[1]->value + player_wo_ace.hand[2]->value;

    //bust function checks
    if (player_bust_check(&non_bust_player)){
        fprintf(stderr, "Busted on the non-bust player, test failed.\n");
        failed = true;
    }
    if (player_bust_check(&player_w_ace)){
        fprintf(stderr, "Busted on the player with an Ace, test failed.\n");
        failed = true;
    }
    if (!player_bust_check(&player_wo_ace)){
        printf("total: %d", player_wo_ace.total);
        fprintf(stderr, "Didn't bust on the player without an Ace, test failed.\n");
        failed = true;
    }
    player_bust(&player_wo_ace);
    if (player_wo_ace.bet != 0 || player_wo_ace.bank != (STARTING_BANK - 15) || player_wo_ace.playing != false){
        fprintf(stderr, "player_bust function test failed.\n");
        failed = true;
    }

    if (failed){
        return 0;
    } else {
        return 1;
    } 
}

//test for the offset function
int test_offset(){
    Player player;
    get_player(&player);
    player.bet = 15.;

    offset(&player);
    if (player.bet != 0.){
        return 0;
    }
    if (player.playing != false){
        return 0;
    }
    return 1;
}

//test for player_new_round function
int test_player_nr(){

    bool failed = false;

    //Initialize players for checks
    Player player_1;
    Player player_2;

    get_player(&player_1);
    get_player(&player_2);

    player_1.bet = 15.;
    player_2.bet = 15.;
    player_2.bank = 0.;

    //check player fields after supposed reset (player_1) or non-reset (player_2)
    player_new_round(&player_1);
    if (player_1.bet != 0){
        fprintf(stderr, "Function didn't reset player 1 correctly.\n");
        failed = true;
    }
    if (player_2.bet != 15.){
        fprintf(stderr, "Function didn;t rest player 2 correctly.\n");
        failed = true;
    }

    if (failed){
        return 0;
    } else {
        return 1;
    } 
}

//test for get_dealer function
int test_get_dealer(){
    Dealer *dealer = get_dealer();
    if (dealer->curr != 0){
        return 0;
    }
    if (dealer->total != 0){
        return 0;
    }
    return 1;
}

//test for dealer_natural check and dealer_natural functions
int test_d_naturals(){

    bool failed = false;

    Dealer *dealer_1 = get_dealer();
    Dealer *dealer_2 = get_dealer();
    //Player player_a;
    //get_player(&player_a);
    //Player player_b;
    //get_player(&player_b);
    //player_a.bet = 15.;
    //player_b.bet = 15.;
    //player_b.total = BLACKJACK;
    dealer_1->total = 21;
    dealer_2->total = 20;

    if (!dealer_natural_check(dealer_1)){
        fprintf(stderr, "dealer 1 natural check did not work correctly.\n");
        failed = true;
    }
    if (dealer_natural_check(dealer_2)){
        fprintf(stderr, "dealer 2 natural check did not work correctly.\n");
        failed = true;
    }
    /*dealer_natural(&player_a);
    if (player_a.bet != 0. || player_a.bank != (STARTING_BANK - 15.)){
        fprintf(stderr, "dealer_natural function failed on player without blackjack.\n");
        failed = true;
    }
    dealer_natural(&player_b);
    if (player_b.bet != 0. || player_b.bank != STARTING_BANK){
        fprintf(stderr, "dealer_natural function failed on player with blackjack.\n");
        failed = true;
    }*/

    if (failed){
        return 0;
    } else {
        return 1;
    }     
}

int test_dealer_busts(){

    bool failed = false;

    //Initialize cards for the checks
    Card *card_ace = get_card(Hearts, Ace, 11);
    Card *card_8 = get_card(Diamonds, Eight, 8);
    Card *card_5 = get_card(Spades, Five, 5);
    Card *card_king = get_card(Spades, King, 10);

    //Initialize dealers for the checks
    Dealer *dealer_w_ace = get_dealer();
    Dealer *dealer_wo_ace = get_dealer();

    dealer_w_ace->hand[0] = card_8;
    dealer_w_ace->hand[1] = card_ace;
    dealer_w_ace->hand[2] = card_5;
    dealer_w_ace->total = 24;

    dealer_wo_ace->hand[0] = card_8;
    dealer_wo_ace->hand[1] = card_5;
    dealer_wo_ace->hand[2] = card_king;
    dealer_wo_ace->total = 23;

    //Initialize players for checks
    Player *players = get_all_players(2);
    players[0].bet = 15.;
    players[0].total = BLACKJACK;
    players[1].bet = 15.;
    players[1].total = 17;

    //check behavior of dealer_bust_check, dealer_ace_behavior, and dealer_bust
    if (dealer_bust_check(dealer_w_ace)){
        fprintf(stderr, "Something wrong with either bust check or ace behavior, busted on player with ace.\n");
        failed = true;
    }
    if (!dealer_bust_check(dealer_wo_ace)){
        fprintf(stderr, "Something wrong with bust check, did not bust on player without ace.\n");
        failed = true;
    }
    dealer_bust(dealer_wo_ace, players, 2);
    if (players[0].bank != (STARTING_BANK + 1.5 * 15.)){
        fprintf(stderr, "dealer_bust check failed with player with blackjack.\n");
        failed = true;
    }
    if (players[1].bank != (STARTING_BANK + 15.)){
        printf("%f\n", players[1].bank);
        fprintf(stderr, "dealer_bust check failed with player with 17.\n");
        failed = true;
    }

    if (failed){
        return 0;
    } else {
        return 1;
    }    
}

int test_end_round(){

    bool failed = false;

    //Initialize dealers for checks
    Dealer *dealer = get_dealer();
    dealer->total = 19;

    //Initialize players for the checks
    Player *players = get_all_players(4);
    players[0].bet = 15.;
    players[0].total = 21;
    players[1].bet = 15.;
    players[1].total = 19;
    players[2].bet = 15.;
    players[2].total = 13;
    players[3].bet = 15.;
    players[3].total = 20;

    //Run checks
    end_round(dealer, players, 4);
    if (players[0].bank != (STARTING_BANK + 1.5 * 15.)){
        fprintf(stderr, "end round test for the blackjack player failed.\n");
        failed = true;
    }
    if (players[1].bank != STARTING_BANK){
        fprintf(stderr, "end round test for the player with same total as dealer failed.\n");
        failed = true;
    }
    if (players[2].bank != (STARTING_BANK - 15.)){
        fprintf(stderr, "end round test for the player with total < dealer total failed.\n");
        failed = true;
    }
    if (players[3].bank != (STARTING_BANK + 15.)){
        fprintf(stderr, "end round test for the player with total > dealer total failed.\n");
        failed  = true;
    }

    if (failed){
        return 0;
    } else {
        return 1;
    }    

}

int test_dealer_hits(){

    bool failed = false;

    //Initialize decks for checks
    Deck *deck_a = get_decks(1);
    Deck *deck_b = get_decks(1);

    //Initialize dealers for checks
    Dealer *dealer_g16 = get_dealer();
    Dealer *dealer_lt17 = get_dealer();
    dealer_g16->total = 17;
    dealer_lt17->total = 14;

    //check for hit 
    if (dealer_hit_check(dealer_g16)){
        fprintf(stderr, "dealer hit check failed to catch dealer total > 16.\n");
        failed = true;
    }
    if (!dealer_hit_check(dealer_lt17)){
        fprintf(stderr, " dealer hit check failed to pass a dealer with total < 17\n");
        failed = true;
    }
Card *down_card = saved_card(deck_a);
dealer_hit_saved(dealer_lt17, down_card);
dealer_hit(dealer_lt17, deck_b);
if (dealer_lt17->hand[0]->value != 1){
    fprintf(stderr, "dealer_hit_saved failed.\n");
    failed = true;
}
if (dealer_lt17->hand[1]->value != 1){
    fprintf(stderr, "dealer_hit failed.\n");
    failed = true;
}

if (failed){
        return 0;
    } else {
        return 1;
    }  

}

int main(int argc, char **argv){
    //check for correct number of arguments
    if (argc < 2){
        fprintf(stderr, "A number between 1 and 8 is required as an argument to run the tests.\n");
        exit(0);
    }

    //conversion of argument into long for use in functions
    char *end;
    long num_decks = strtol(argv[1], &end, 10);
    
    //Check to make sure number entered as argument is within the allowed number of decks for a game of blackjack
    if (num_decks < 1 || num_decks > 8){
        fprintf(stderr, "Enter a number between 1 and 8 as an arugment.\n");
        exit(0);
    }

    //get_card function test
    if (test_get_card((Suit) 3, (Rank) 1, 11)){
        printf("get_card function tests passed.\n");
    } else {
        printf("get_card function tests failed.\n");
        return 0;
    }

    //get_decks function test
    if (test_get_decks(num_decks)){
        printf("get_decks function tests passed with %ld deck.\n", num_decks);
    } else {
        printf("get_decks function tests failed with %ld deck.\n", num_decks);
        return 0;
    }

    //shuffle function test
    if (test_shuffle(num_decks)){
        printf("shuffle function test passed with %ld deck.\n", num_decks);
    } else {
        printf("The shuffle wasn't good enough, run again to see if this was by chance.\n");
    }

    //pop function test
    if (test_pop(num_decks)){
        printf("pop function test passed with %ld deck.\n", num_decks);
    } else {
        printf("pop function test failed with %ld deck.\n", num_decks);
        return 0;
    }

    //get_player function test
    if (test_get_player()){
        printf("get_player function test passed.\n");
    } else {
        printf("get_player function test failed.\n");
        return 0;
    }

    //bet function test
    if (test_wager((14))){
        printf("wager function test passed.\n");
    } else {
        printf("wager function test failed.\n");
        return 0;
    }

    //player_hit function test
    if (test_player_hit(num_decks)){
        printf("player_hit function test passed.\n");
    } else {
        printf("player_hit function test failed.\n");
        return 0;  
    }

    //player_natural_check and player_natural functions test
    if (test_natural()){
        printf("natural functions test passed.\n");
    } else {
        printf("natural functions test failed.\n");
        return 0;
    }

    //player_bust_check and player_bust functions test
    if (test_bust()){
        printf("bust functions test passed.\n");
    } else {
        printf("bust functions test failed.\n");
        return 0;
    }

    //offset function test
    if (test_offset()){
        printf("offset functions test passed.\n");
    } else {
        printf("offset functions test failed.\n");
    }

    //player_new_round function test
    if (test_player_nr()){
        printf("player_new_round function test passed.\n");
    } else {
        printf("player_new_round function test failed.\n");
    }

    //get_dealer function test
    if (test_get_dealer()){
        printf("get_dealer function test passed.\n");
    } else {
        printf("get_dealer function test passed.\n");
    }

    //dealer_natural_check and dealer_natural functions test
    if (test_d_naturals()){
        printf("dealer natural functions test passed.\n");
    } else {
        printf("dealer natural functions test failed.\n");
        return 0;
    }

    //dealer_bust_check, dealer_ace_behavior, and dealer_bust funtions test
    if (test_dealer_busts()){
        printf("dealer bust and ace behavior functions test passed.\n");
    } else {
        printf("dealer bust and ace behavior functions test failed.\n");
        return 0;  
    }

    //end_round function test
    if (test_end_round()){
        printf("end_round function test passed.\n");
    } else {
        printf("end_round function test failed.\n");
        return 0;
    }

    //dealer_hit_check, saved_card, dealer_hit_saved, and dealer_hit functions test
    if (test_dealer_hits()){
        printf("dealer hit functions test passed.\n");
    } else {
        printf("dealer hit functions test failed.\n");
        return 0;
    }

    printf("Passed all tests! :)\n");
    return 1;
}