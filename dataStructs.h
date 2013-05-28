#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define 	SHUFFLE_TIMES	256
#define 	FIRST_NUMERAL 	2
#define 	DECK_SIZE 		52

typedef struct CARD {
	char suit;
	char rank[3];
} CARD;

typedef struct DECK {
	CARD cards[DECK_SIZE];
	int size;
} DECK;

typedef struct PLAYER {
	int number;
	CARD hand[26];
	char name[15];
	char FIFOname[16];
	int FIFOfd;
} PLAYER;

typedef struct GAMEINFO {
	PLAYER players[8];
	int currPlayers;
	int nPlayers;
	int pturn;
	int round;
	int dealer;
	DECK table;
	pthread_mutex_t gameStart_mut;
	pthread_cond_t gameStart_cond;
} GAMEINFO;

// Auxiliary Methods
char *itoa( int n );

// PLAYER Methods
void PLAYER_init( PLAYER *p, char *pName, char *FIFOname, int number, int FIFOfd );

// DECK Methods
void DECK_init(DECK *deck);
void DECK_shuffle( DECK *deck );

// Shared Memory Methods
GAMEINFO *shmM_create( char *shm_name, int shm_size );
GAMEINFO *shmM_attach( char *shm_name, int shm_size );
void shmM_destroy( GAMEINFO *shm, char *shm_name, int shm_size );
