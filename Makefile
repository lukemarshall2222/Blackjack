# Makefile for the C Blackjack game

all:
	clang -W -Wall -g -o Blackjack Blackjack_functions.c Blackjack.c

exec:
	./Blackjack

test:
	clang -W -Wall -g -o BJ_test Blackjack_functions.c Blackjack_tests.c

run_test:
	./BJ_test 4