typedef struct card {
	char *suit;
	char *value;
} CARD;

typedef struct deck {
	CARD *cards;
	int size;
} DECK;

typedef struct player {
	CARD *hand;
	char *name;
} PLAYER;

void printCard(CARD c);

void printDeck(DECK d);

void printPlayerInfo(PLAYER p);
void printPlayerHand(PLAYER p);
void printPlayerName(PLAYER p);
