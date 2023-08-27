/* In this program we used the Windows.h library which provided us with two
	functions: Sleep(); and system("cls"); Sleep pauses the program for a set 
	of time and system("cls") clears the program's console. We used both of these
	functions to make the program cleaner and easier to understand from the player's
	perspective. Our program purposely stops after displaying certain messages before
	clearing the console to allow the players to read the different messages. However, 
	the windows.h library is not available in other operating systems, which is why we
	submitted a separate version of our program which is not as clean, but works on 
	all operating systems.
*/
# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <stdbool.h>
# include <Windows.h> //Imports window's library for animation, does not work on mac.



typedef struct card_s {		//creates struct for the playing deck and discard pile
	char color[7];
	int value;
	char action[15];
	struct card_s* pt;
} card;

typedef struct player_one {		//creates struct to implement player one's hand as a linked list
	char name[20];
	char color[7];
	int value;
	char action[15];
	struct player_one* pt;
} player1;

typedef struct player_two {		//creates struct to implement player two's hand as a linked list
	char name[20];
	char color[7];
	int value;
	char action[15];
	struct player_two* pt;
} player2;

typedef struct currcard {		//creates struct to save the current playing card
	char color[7];
	int value;
	char action[15];
} temp;

void Welcome();		//Prints a welcome message to start the game
void PopulateDeck(FILE* inp, card* x);		//reads cards from file into the card deck
void shuffleCards(card* x);			//shuffles cards in the deck
void dealCards(card* x, player1* o, player2* t, card* headp);	//deals 7 cards to each player
bool placeCard1(player1* x, temp y, int selection);		//determines if the selected card by player 1 can be placed
bool placeCard2(player2* x, temp y, int selection);		//determines if the selected card  by player 2 can be placed
void draw1(player1* x, card* deck, int n, int* numcards);	//gets the next card from the playing deck and adds it to the player one's hand
void draw2(player2* x, card* deck, int n, int* numcards);	//gets the next card from the playing deck and adds it to the player two's hand
void printCurrentDeck1(player1* x, int numCards);		//Prints player one's current hand
void printCurrentDeck2(player2* x, int numCards);		//Prints player two's urrent hand
void printCurrentCard(temp x);		//Prints the current card at play
void plusFour(player1* x, player2* y, card* deck, int* p1n, int* p2n, int* l, int c);		//Adds the next four cards in the playing deck to a player's hand
void changeColor(temp* x, char color);		//changes the color of the card at play to selected color if a plusfourWild or Wild card is placed
void plusTwo(player1* x, player2* y, card* deck, int* p1n, int* p2n, int* l, int c);		//Adds the next two cards in the playing deck to a player's hand
void removeCardFromDeck(card* x, card* headp, int i);		//Removes dealed card from the deck
void removecardFromHand1(player1* p_one, int selection, player1* head);		//removes placed card from player one's hand
void removecardFromHand2(player2* p_two, int selection, player2* head);		//removes placed card from player two's hand
void addCardToHand1(int numCards, int* l, card* deck, player1* p_one, int add);		//adds card or cards to player one's hand
void addCardToHand2(int numCards, int* l, card* deck, player1* p_two, int add);		//adds card or card to player two's hand
void UNO();					//prints a message when one of the players has one card left
void moveToPile(card* discard, temp currCard, int* p);		//moves every placed card into the discard pile
void pileToDeck(card* pile, card* deck);		//Moves the Discard Pile into the playing deck once all cards from the playing deck have been drawed
void shuffleDiscardPile(card* x, int y);		//Shuffles the card in the discard pile

int main(void) {
	FILE* inp;							//will hold the file containing the playing cards
	card* deck, * headp;				//holds the playing deck and the head pointer to the deck
	player1* p_one, * headp1;			//gets the next card from the playing deck and adds it to the player one's hand
	player2* p_two, * headp2;			//Holds the head pointer to player 2's hand, holds player 2's hand
	card* discardPile;					//Holds the discard Pile
	temp currentCard;					//Holds the card at play
	int numCards = 108;					//total number of cards in the deck
	int i, l = 14;						//l holds the current card number in the deck after the cards have been delt to both players
	int p1numCards = 7, p2numCards = 7;		//Holds player one's and player two's number of cards at hand, starting with 7 which is the initial number of cards delt to each player
	int selection;						//holds the player's selection in terms of the player's hand
	char change = '\n';				//holds the player selection in terms of color when placing a Wild card
	int skip1 = 0, skip2 = 0;		//holds a value to determine if a player is skipped
	int pileCounter = 0;			//Holds the total number of cards in the discard pile

	inp = fopen("cards.txt", "r");		//opens file of cards

	//dynamically allocates memory for both players' hands, playing deck and discard pile
	deck = (card*)malloc(numCards * sizeof(card));
	p_one = (player1*)malloc(numCards * sizeof(player1));
	p_two = (player2*)malloc(numCards * sizeof(player2));
	discardPile = (card*)malloc(numCards * sizeof(card));


	if (inp == NULL) {
		return -1;		//returns an error if the card file does not exist
	}

	//sets the head pointers equal to the address of the first items in each list
	headp = &deck[0];
	headp1 = &p_one[0];
	headp2 = &p_two[0];

	PopulateDeck(inp, deck);								//reads the cards from file into the card deck
	shuffleCards(deck);										//shuffles the cards in the deck
	fclose(inp);											//closes the file of cards after all cards have been read into the deck

	//Prints a welcome message and asks for the players names
		printf("\tWelcome to the game of UNO!!!\n");
		Welcome();
		system("cls");
		printf("Player One, Enter your name:\n");
		fgets(p_one[0].name, 20, stdin);
		printf("Player Two, Enter your name:\n");
		fgets(p_two[0].name, 20, stdin);

	
	system("cls");
	dealCards(deck, p_one, p_two, headp);		//deals cards from the deck into the players' hands


	//removes the newline at the end of both players names
	p_one[0].name[strlen(p_one[0].name) - 1] = NULL;
	p_two[0].name[strlen(p_two[0].name) - 1] = NULL;

	//Prints player one's deck
	printf("\n\n%s this is your deck:\n", p_one[0].name);
	for (i = 0; i < 7; ++i) {
		if (p_one[i].value < 10) {
			printf("(%d) %s %d\n", i + 1, p_one[i].color, p_one[i].value);
		}
		else {
			printf("(%d) %s %s\n", i + 1, p_one[i].color, p_one[i].action);
		}
	}
	Sleep(2000);
	system("cls");

	//Prints player two's deck
	printf("\n\n%s this is your deck:\n", p_two[0].name);
	for (i = 0; i < 7; ++i) {
		if (p_two[i].value < 10) {
			printf("(%d) %s %d\n", i + 1, p_two[i].color, p_two[i].value);
		}
		else {
			printf("(%d) %s %s\n", i + 1, p_two[i].color, p_two[i].action);
		}
	}

	Sleep(2000);
	system("cls");

	//Prints the first card at play
	printf("\n\n\nAnd the First Card is .....\n");
	Sleep(600);
	while (deck[l].value > 12) {		//guarantees first card is not a Wild card or plus four
		++l;
	}
	if (deck[l].value < 10) {
		printf("%s %d\n\n", deck[l].color, deck[l].value);
	}
	else {
		printf("%s %s\n\n", deck[l].color, deck[l].action);
	}

	//sets current card to the first card at play
	strcpy(currentCard.color, deck[l].color);
	currentCard.value = deck[l].value;
	strcpy(currentCard.action, deck[l].action);
	moveToPile(discardPile, currentCard, &pileCounter);
	Sleep(600);


	while (p1numCards != 0 && p2numCards != 0) {		//Game loop, only stops if a player has no remaining cards or a player chooses to forfeit

		skip2 = 0;		//sets skip2 equal to zero each iteration so player 2 is not skipped more than once in a row

		if (skip1 == 0) {		//checks if player one should be skipped

			//Prints player one's current deck and prompts them to select a card
			printCurrentDeck1(p_one, p1numCards);
			printf("%s it's your turn! Pick a card to place down (1-%d), enter 0 to draw, or enter -1 to forfeit:", p_one[0].name, p1numCards);
			scanf("%d", &selection);
			change = '\n';


			while (!placeCard1(p_one, currentCard, selection)) { //checks if selected card can be placed 
				//if card can't be placed loop will keep prompting to select another card or draw

				system("cls");

				if (selection == -1) {	//ends game if player forfeits
					p2numCards = 0;
					break;
				}
				if (selection == 0) {	//checks if player draws a card
					++l;					//adds one to current card in playing deck

					if (l == 108) {			//checks if the playing deck has reached the end
						printf("You hace reached the end of the Deck!!\n");
						printf("Shuffling discard pile...\n");
						shuffleDiscardPile(discardPile, pileCounter);		//will shuffle the discard pile
						Sleep(1000);
						pileToDeck(discardPile, deck);		//Moves the discard pile into the playing deck to keep the game going
						l = 0;		//restarts the counter of the current card in the deck
					}

					draw1(p_one, deck, l, &p1numCards);		//draws one card from the playing deck into the player's hand

					//Prints the card drawn
					if (p_one[p1numCards - 1].value < 10) {
						printf("\nYou got: %s %d\n", p_one[p1numCards - 1].color, p_one[p1numCards - 1].value);
					}
					else {
						printf("\nYou got: %s %s\n", p_one[p1numCards - 1].color, p_one[p1numCards - 1].action);
					}

					printCurrentDeck1(p_one, p1numCards);
					printCurrentCard(currentCard);

				}

				//Prompts to select another card or forfeit if selected card cannot be placed
				if (selection != 0) {
					printCurrentDeck1(p_one, p1numCards);
					printCurrentCard(currentCard);
					printf("\nCard cannot be placed, Card must match Color, Number or Action of Current Card.\n");
					printf("Select another card, enter 0 to draw or enter -1 to forfeit:\n");
					scanf("%d", &selection);
				}
				else {
					printf("Pick a card to place down (1-%d), enter 0 to draw or enter -1 to forfeit:", p1numCards);
					scanf("%d", &selection);
				}
			}

			printf("Card Placed!\n");		//once card has been placed displays a confirmation message

			if (p1numCards == 0 || p2numCards == 0) {	//if one of the players has won the loop breaks, ending the game
				break;
			}

			Sleep(600);

			if (p_one[selection - 1].value == 14) {			//checks if card is a plusFour Wild Card
				printf("PLUS FOUR!!!");
				plusFour(p_one, p_two, deck, &p1numCards, &p2numCards, &l, 0);		//adds four cards to other player's hand
				printf("Choose a color (r,g,b, or y):");
				while (change == '\n') {
					scanf("%c", &change);					//gets the player's color selection
				}
				changeColor(&currentCard, change);			//changes color to player's selection
			}

			if (p_one[selection - 1].value == 12 || p_one[selection - 1].value == 10) {		//checks if card is a reverse or skip
				Sleep(700);
				if (p_one[selection - 1].value == 12) {
					printf("SKIP!!\n");
				}
				else {
					printf("Reverse -> <-\n");
				}
				skip2 = 1;				//will skip player 2's turn
			}

			if (p_one[selection - 1].value == 11) {				//checks if card is a plus two
				printf("PLUS TWO!!\n");
				plusTwo(p_one, p_two, deck, &p1numCards, &p2numCards, &l, 0);		//adds two to other player's hand
			}

			if (p_one[selection - 1].value == 13) {				//checks if card is a wild card
				printf("Choose a color (r,g,b, or y):");
				while (change == '\n') {
					scanf("%c", &change);					//gets player's color selection
				}

				changeColor(&currentCard, change);		//changes color to player's selection
			}
			system("cls");

			--p1numCards;		//once card has been placed, subtracts one from player 1's number of cards

			if (p1numCards == 1) {		//prints a message if player 1 has 1 card left
				UNO();
			}

			change = '\n';

			if (p_one[selection - 1].value != 14 && p_one[selection - 1].value != 13) {		//sets the card at play equal to the placed card
				strcpy(currentCard.color, p_one[selection - 1].color);
				currentCard.value = p_one[selection - 1].value;
				strcpy(currentCard.action, p_one[selection - 1].action);
			}

			removecardFromHand1(p_one, selection, headp1);
			moveToPile(discardPile, currentCard, &pileCounter);
		}

		skip1 = 0;		//sets skip1 equal to zero so player 1 is not skipped more than once in a row


		if (p1numCards == 0) {		//breaks loop and ends game if p1 has zero cards left
			break;
		}

		if (skip2 == 1) {
			printCurrentCard(currentCard);			//if player 2 is skipped prints card at play to remind player one what it is
		}

		

		if (skip2 == 0) {			//checks if player two must be skipped
			//prompts player 2 to select a card
			printCurrentCard(currentCard);
			printCurrentDeck2(p_two, p2numCards);
			printf("%s it's your turn!! Pick a card to place down (1-%d), enter 0 to draw or enter -1 to forfeit:", p_two[0].name, p2numCards);
			scanf("%d", &selection);

			while (!placeCard2(p_two, currentCard, selection)) { //checks if selected card can be placed

				system("cls");

				if (selection == -1) {	//if player 2 forfeits the loop breaks and game ends
					p1numCards = 0;
					break;
				}

				if (selection == 0) {	//checks if player 2 chooses to draw a card

					++l;		//adds one to the card number in the deck in order to get next card

					if (l == 108) {			//checks if deck has reached the end and shuffles discard pile to move it into the playing deck
						printf("You hace reached the end of the Deck!!\n");
						printf("Shuffling discard pile...\n");
						Sleep(1000);
						pileToDeck(discardPile, deck);
						l = 0;		//sets card number in the deck equal to zero
					}

					draw1(p_two, deck, l, &p2numCards);		//draws a card into player 2's hand

					//prints the card drawn
					if (p_two[p2numCards - 1].value < 10) {
						printf("\nYou got: %s %d\n", p_two[p2numCards - 1].color, p_two[p2numCards - 1].value);
					}
					else {
						printf("\nYou got: %s %s\n", p_two[p2numCards - 1].color, p_two[p2numCards - 1].action);
					}

					printCurrentDeck2(p_two, p2numCards);
					printCurrentCard(currentCard);

				}

				//prompts user to select another card or draw, since card cannot be placed
				if (selection != 0) {
					printCurrentDeck2(p_two, p2numCards);
					printCurrentCard(currentCard);
					printf("\nCard cannot be placed, Card must match Color, Number or Action of Current Card.\n");
					printf("Select another card, enter 0 to draw or enter -1 to forfeit:\n");
					scanf("%d", &selection);
				}
				else {
					printf("Pick a card to place down (1-%d), enter 0 to draw or enter -1 to forfeit:", p2numCards);
					scanf("%d", &selection);
				}
			}

			//once again checks if playing deck has reached the end to avoid crashing in next iteration if player one tries drawing a card
			if (l == 108) {
				printf("You hace reached the end of the Deck!!\n");
				printf("Shuffling discard pile...\n");
				shuffleDiscardPile(discardPile, pileCounter);
				Sleep(2000);
				pileToDeck(discardPile, deck);
				l = 0;
			}

			if (p1numCards == 0) {		//gives the win to player 1 if player two forfeits.
				break;
			}

			printf("Card Placed!\n");	//Confirms card has been placed

			Sleep(600);

			if (p_two[selection - 1].value == 14) {			//checks if selected card is a plusFour Wild Card
				plusFour(p_one, p_two, deck, &p1numCards, &p2numCards, &l, 1);
				printf("PLUS FOUR!!!");
				printf("Choose a color (r,g,b, or y):");
				while (change == '\n') {
					scanf("%c", &change);					//gets player 2's selection of color
				}
				changeColor(&currentCard, change);
			}

			if (p_two[selection - 1].value == 12 || p_two[selection - 1].value == 10) {	//checks if card is a skip or reverse card
				Sleep(700);
				if (p_two[selection - 1].value == 12) {
					printf("SKIP!!\n");
				}
				else {
					printf("Reverse -> <-\n");
				}
				skip1 = 1;			//sets skip1 equal to 1 to skip player 1's turn
			}

			if (p_two[selection - 1].value == 11) {		//checks if selected card is a plus two card
				printf("PLUS TWO!!");
				plusTwo(p_one, p_two, deck, &p1numCards, &p2numCards, &l, 1);	//adds two to other player's hand
			}

			if (p_two[selection - 1].value == 13) {		//checks if selected card is a wild card
				printf("Choose a color (r,g,b, or y):");
				while (change == '\n') {
					scanf("%c", &change);			//gets player 2's selection of color
				}

				changeColor(&currentCard, change);		//changes color to player 2's selection
			}

			system("cls");

			--p2numCards;	//After card is placed decreases number of cards in player 2's hands by 1

			if (p2numCards == 1) {		//If player 2 has 1 card left will print a message
				UNO();
			}

			if (p_two[selection - 1].value != 14 && p_two[selection - 1].value != 13) {		//sets card at play equal to the card placed
				strcpy(currentCard.color, p_two[selection - 1].color);
				currentCard.value = p_two[selection - 1].value;
				strcpy(currentCard.action, p_two[selection - 1].action);
			}

			removecardFromHand2(p_two, selection, headp2);		//removes placed card from player 2's hand

			if (skip1 == 0) {
				printCurrentCard(currentCard);			//if player 1 is not to be skipped prints the current card to remind player 1 of the card at play
			}

			moveToPile(discardPile, currentCard, &pileCounter);		//moves placed card into discard Pile
		}

	}

	system("cls");

	if (p1numCards == 0) {			//checks if p1 won the game
		if (selection == -1) {		//prints a message if the game was won by forfeit
			printf("%s has forfeited.\n", p_two[0].name);
		}
		printf("%s HAS WON!!!\n", p_one[0].name);
	
	}

	else if (p2numCards == 0) {		//checks if p2 won the game
		if (selection == -1) {		//prints a message if the game was won by forfeit
			printf("%s has forfeited.\n", p_one[0].name);
		}
		printf("%s HAS WON!!!\n", p_two[0].name);
	
	}

	//frees all of the memory that was allocated for the game
	free(deck);
	free(p_one);
	free(p_two);
	free(discardPile);

	return 0;
}


void PopulateDeck(FILE* inp, card* x) {
	int i = 0;
	while (!feof(inp)) {
		fscanf(inp, "%s %d %s", x[i].color, &x[i].value, x[i].action);			//scans cards from file into deck
		if (i + 1 != 108) {
			x[i].pt = &x[i + 1];
		}
		else {
			x[i].pt = NULL;
		}
		++i;
	}
}

void shuffleCards(card* x) {
	int rand1, rand2, i;
	card temp;
	srand(time(0));

	for (i = 10000; i > 0; i--) {    //runs 10000 for good shuffling and to ensure all elements get shuffled
		rand1 = rand() % 108;		//next two lines get random number form 0 - 107
		rand2 = rand() % 108;
		temp = x[rand1];
		x[rand1] = x[rand2];
		x[rand2] = temp;
	}
}

void shuffleDiscardPile(card* x, int y) {
	int rand1, rand2, i;
	card temp;
	srand(time(0));

	for (i = 10000; i > 0; i--) {    //runs 10000 for good shuffling and to ensure all elements get shuffled
		rand1 = rand() % y;		//next two lines get random number form 0 to however many cards are in the discard pile
		rand2 = rand() % y;
		temp = x[rand1];
		x[rand1] = x[rand2];
		x[rand2] = temp;
	}
}

void dealCards(card* x, player1* o, player2* t, card* headp) {
	int i, j = 0;
	for (i = 0; i < 7; ++i) {
		strcpy(o[i].color, x[i].color);					//gets the shuffled cards into the player one's hand from the deck
		strcpy(o[i].action, x[i].action);
		o[i].value = x[i].value;
		if (i < 6) {
			o[i].pt = &o[i + 1];
		}
		else {
			o[i].pt = NULL;
		}
		removeCardFromDeck(x, headp, i);				//removes dealt cards from the playing deck
	}
	for (i = 14; j < 7; ++i) {
		strcpy(t[j].color, x[i].color);
		strcpy(t[j].action, x[i].action);		//deals 7 card into player 2's hand from the playing deck
		t[j].value = x[i].value;
		if (j < 6) {
			t[j].pt = &t[j + 1];
		}
		else {
			t[j].pt = NULL;
		}
		++j;
		removeCardFromDeck(x, headp, i);					//removes dealt cards from the playing deck
	}
}

bool placeCard1(player1* x, temp y, int selection) {
	if (selection == 0) {									//checks if selected card can be placed by seeing if it matches color, number or action (except play)
		return false;
	}
	if (strcmp(x[selection - 1].color, y.color) == 0) {
		return true;
	}
	else if (x[selection - 1].value == y.value) {
		return true;
	}
	else if (x[selection - 1].value > 12) {
		return true;
	}
	else {
		return false;
	}
}
bool placeCard2(player2* x, temp y, int selection) {		//checks if selected card can be placed by comparing current card at play to selected card
	if (selection == 0) {
		return false;
	}
	if (strcmp(x[selection - 1].color, y.color) == 0) {
		return true;
	}
	else if (x[selection - 1].value == y.value) {
		return true;
	}
	else if (x[selection - 1].value > 12) {
		return true;
	}
	else {
		return false;
	}
}
void draw1(player1* x, card* deck, int n, int* numcards) {
	int* dummy = &n;
	addCardToHand1(*numcards, dummy, deck, x, 1);		//adds a card to player 1's hand
	(*numcards)++;			//increases number of cards in the player's hand
}
void draw2(player2* x, card* deck, int n, int* numcards) {
	int* dummy = &n;
	addCardToHand2(*numcards, dummy, deck, x, 1);		//adds a card to player 2's hand
	(*numcards)++;			//increases number of cards in the player's hand
}
void printCurrentDeck1(player1* x, int numCards) {
	int i = 0;
	printf("%s's current deck is:\n", x[0].name);		//prints the player 1's hand
	//the following statements format the printing for a more intuitive program
	if (x[i].value > 9) {
		printf("(%d) %s %s\n", i + 1, x[i].color, x[i].action);
	}
	else {
		printf("(%d) %s %d\n", i + 1, x[i].color, x[i].value);
	}

	while (x[i].pt != NULL) {
		if (x[i].pt->value < 10) {
			printf("(%d) %s %d\n", i + 2, x[i].pt->color, x[i].pt->value);
		}
		else {
			printf("(%d) %s %s\n", i + 2, x[i].pt->color, x[i].pt->action);
		}
		++i;
	}
}
void printCurrentDeck2(player2* x, int numCards) {				//prints player 2's hand
	int i = 0;			
	printf("%s's current deck is:\n", x[0].name);
	//the following statements format the printing to make the program more intuitive
	if (x[i].value > 9) {
		printf("(%d) %s %s\n", i + 1, x[i].color, x[i].action);
	}
	else {
		printf("(%d) %s %d\n", i + 1, x[i].color, x[i].value);
	}

	while (x[i].pt != NULL) {
		if (x[i].pt->value < 10) {
			printf("(%d) %s %d\n", i + 2, x[i].pt->color, x[i].pt->value);
		}
		else {
			printf("(%d) %s %s\n", i + 2, x[i].pt->color, x[i].pt->action);
		}
		++i;
	}
}

void printCurrentCard(temp x) {		//prints the current card at play
	printf("\nCurrent Card: ");
	//the following statements format the printing to make the program more intuitive
	if (x.value < 10) {
		printf("%s %d\n\n", x.color, x.value);
	}
	else {
		printf("%s %s\n\n", x.color, x.action);
	}
}

void plusFour(player1* x, player2* y, card* deck, int* p1n, int* p2n, int* l, int c) {
	//This function adds four cards to a players hand by calling on another function, this function is used to pass the information into the add function
	//Although this function is not really necessary it makes the main a bit clearer
	//this function also updates the players' number of cards
	int i;
	if (c == 0) {
		addCardToHand2(*p2n, l, deck, y, 4);
		*p2n += 4;
	}
	else {
		addCardToHand1(*p1n, l, deck, x, 4);
		*p1n += 4;
	}
}

void changeColor(temp* x, char color) {
	//checks the player's color selection and changes the current card at play the same color as the player's selection
	if (color == 'r') {
		strcpy(x->color, "Red");
	}
	if (color == 'b') {
		strcpy(x->color, "Blue");
	}
	if (color == 'y') {
		strcpy(x->color, "Yellow");
	}
	if (color == 'g') {
		strcpy(x->color, "Green");
	}
	strcpy(x->action, "Play");
	x->value = 15;		//makes it so that the next card cannot match the value and forcefully has to match the color
}

void plusTwo(player1* x, player2* y, card* deck, int* p1n, int* p2n, int* l, int c) {
	//This function adds two cards to a players hand by calling on another function, this function is used to pass the information into the add function
	//Although this function is not really necessary it makes the main a bit clearer
	//this function also updates the players' number of cards
	int i;
	if (c == 0) {
		addCardToHand2(*p2n, l, deck, y, 2);
		*p2n += 2;
	}
	else {
		addCardToHand1(*p1n, l, deck, x, 2);
		*p1n += 2;
	}

}
void removeCardFromDeck(card* x, card* headp, int i) {
	card* temp;
	if (i != 0) {
		x[i - 1].pt = &x[i + 1];	//makes the card before point to the card after the one being removed
	}
	if (i = 0) {
		headp = &x[i + 1];			//if the card being removed is the first in the deck, the head pointer will be moved to the next card
	}

}

void removecardFromHand1(player1* p_one, int selection, player1* head) {
	selection = selection - 1;		//turns the selection into the index
	if (p_one[selection].pt != NULL) {		//checks if card is the last in the deck
		while (p_one[selection].pt != NULL) {		//pushes linked list down to fill spot of removed card if card isn't the last one
			strcpy(p_one[selection].color, p_one[selection].pt->color);
			p_one[selection].value = p_one[selection].pt->value;
			strcpy(p_one[selection].action, p_one[selection].pt->action);
			++selection;
		}
		p_one[selection - 1].pt = NULL;		//sets last card's pt equal to null
	}
	else {
		p_one[selection - 1].pt = NULL;		//removes last card from the linked list if it is the one being removed
	}

}
void removecardFromHand2(player2* p_two, int selection, player2* head) {
	selection = selection - 1;			//turns the selection into the index
	if (p_two[selection].pt != NULL) {		//checks if card is the last one in the player's hand
		while (p_two[selection].pt != NULL) {		//if it is not the last card it will push the list down to fill the spot of the removed list 
			strcpy(p_two[selection].color, p_two[selection].pt->color);
			p_two[selection].value = p_two[selection].pt->value;
			strcpy(p_two[selection].action, p_two[selection].pt->action);
			++selection;
		}
		p_two[selection - 1].pt = NULL;		//sets the pointer of the last card equal to NULL
	}
	else {
		p_two[selection - 1].pt = NULL;		//removes last card from the linked list if it is the one being removed
	}

}

void addCardToHand1(int numCards, int* l, card* deck, player1* p_one, int add) {
	int i;
	p_one[numCards - 1].pt = &p_one[numCards];		//Adds a pointer from the last card in player's hand to the one being added
	for (i = 0; i < add; ++i) {
		//following statements add the cards to the player's hand from the deck linking them to the list
		strcpy(p_one[numCards + i].color, deck[*l].color);
		strcpy(p_one[numCards + i].action, deck[*l].action);
		p_one[numCards + i].value = deck[*l].value;
		p_one[numCards + i].pt = &p_one[numCards + i + 1];
		++(*l);			//updates the number of cards used from the playing deck

	}
	p_one[numCards + i - 1].pt = NULL;	//Sets the pointer of the last item equal to NULL

}

void addCardToHand2(int numCards, int* l, card* deck, player1* p_two, int add) {
	int i;
	p_two[numCards - 1].pt = &p_two[numCards];		//Adds a pointer from the last card in player's hand to the one being added
	for (i = 0; i < add; ++i) {
		//following statements add the cards to the player's hand from the deck linking them to the list
		strcpy(p_two[numCards + i].color, deck[*l].color);
		strcpy(p_two[numCards + i].action, deck[*l].action);
		p_two[numCards + i].value = deck[*l].value;
		p_two[numCards + i].pt = &p_two[numCards + i + 1];
		++(*l);			//updates the number of cards used from the playing deck

	}
	p_two[numCards + i - 1].pt = NULL;		//sets the pointer of the last item equal to NULL
}

void moveToPile(card* discard, temp currCard, int* p) {
	//moves placed cards into the discard pile
	strcpy(discard[*p].color, currCard.color);		
	strcpy(discard[*p].action, currCard.action);
	discard[*p].value = currCard.value;
	//links cards in the discard pile
	discard[*p].pt = NULL;
	if (*p > 0) {
		discard[*p - 1].pt = &discard[*p];
	}

	(*p)++;	//updates number of cards in the discard pile
}

void pileToDeck(card* pile, card* deck) {
	int i = 0;
	while (pile[i].pt != NULL) {			//moves the shuffled discard pile into the playing deck to keep the game going
		strcpy(deck[i].color, pile[i].color);
		strcpy(deck[i].action, pile[i].action);
		deck[i].value = pile[i].value;
		++i;
	}
}

void Welcome() {												//prints a welcome message 
	printf("\t****************************\n");
	printf("\t**          **            **\n");
	printf("\t**        **              **\n");
	printf("\t**      **                **\n");
	printf("\t**    **     **           **\n");
	printf("\t**   *      ***           **\n");
	printf("\t**  *      * **           **\n");
	printf("\t** *         **          ***\n");
	printf("\t** *         **         ****\n");
	printf("\t***          **        *  **\n");
	printf("\t***          **       *   **\n");
	printf("\t**           **      *    **\n");
	printf("\t**                  *     **\n");
	printf("\t**               **       **\n");
	printf("\t**            **          **\n");
	printf("\t**         ***            **\n");
	printf("\t****************************\n");
	Sleep(1500);
	system("cls");
	printf("\t  **          **    ****        **      *********               \n");
	printf("\t  **          **    ** **       **     ***     ***              \n");
	printf("\t  **          **    **  **      **     **        **             \n");
	printf("\t  **          **    **   **     **    **          **            \n");
	printf("\t  **          **    **    **    **    **          **            \n");
	printf("\t  **          **    **     **   **    **          **            \n");
	printf("\t  **          **    **      **  **     **        **             \n");
	printf("\t   ***      ***     **       ** **     ***      ***             \n");
	printf("\t    **********      **        ****      *********               \n");
	printf("\t\tDEVELOPED BY: JORGE LUJAN & GERARDO RIOS\n");
	Sleep(1500);
	system("cls");

}

void UNO() {		//prints UNO when one of the players has 1 card left
	printf("  **          **    ****        **      *********               \n");
	printf("  **          **    ** **       **     ***     ***              \n");
	printf("  **          **    **  **      **     **        **             \n");
	printf("  **          **    **   **     **    **          **            \n");
	printf("  **          **    **    **    **    **          **            \n");
	printf("  **          **    **     **   **    **          **            \n");
	printf("  **          **    **      **  **     **        **             \n");
	printf("   ***      ***     **       ** **     ***      ***             \n");
	printf("    **********      **        ****      *********               \n");
	Sleep(1500);
	system("cls");
}

