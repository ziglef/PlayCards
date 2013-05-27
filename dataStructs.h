#define 	SHUFFLE_TIMES	256
#define 	FIRST_NUMERAL 	2
#define 	DECK_SIZE 		52

typedef struct {
	char suit;
	char *rank;
} CARD;

typedef struct {
	CARD *cards;
	int size;
} DECK;

typedef struct {
	int number;
	CARD *hand;
	char *name;
	char *FIFOname;
} PLAYER;

typedef struct {
	PLAYER *players;
	int nplayers;
	int pturn;
	int round;
	int dealer;
	CARD *table;
} GAMEINFO;

char *itoa(int n);
DECK *DECK_init();
void DECK_shuffle(DECK *deck);

GAMEINFO *shmM_open_create( char *shm_name, int shm_size );
void shmM_destroy(GAMEINFO *shm, char *shm_name, int shm_size);
