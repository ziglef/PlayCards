#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <limits.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define 	SHUFFLE_TIMES		256
#define 	FIRST_NUMERAL 		2
#define 	DECK_SIZE 			52
#define		PLAYER_MAX			8
#define 	MAX_HAND_SIZE 		26
#define 	NUM_OF_CHARACTERS	20


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
	CARD hand[MAX_HAND_SIZE];
	int hand_size;
	char name[15];
	char FIFOname[16];
} PLAYER;

typedef struct GAMEINFO {
	PLAYER players[8];
	pid_t PPID[8];
	int currPlayers;
	int nPlayers;
	int pturn;
	int currRound;
	int dealer;
	DECK table;
	
	int GAMERUNNING;
	
	pthread_mutex_t GAMEFLAGS_MUT;
	pthread_mutex_t GAMESTART_MUT;
	pthread_mutex_t SCREEN_WRITE;
	pthread_mutex_t PPERM[8];
	pthread_mutex_t PPERM_READ[8];
	
	char current_play[256];
	char old_play[256];
} GAMEINFO;

// Auxiliary Methods
char *itoa( int n );
void getDate( char *when );

// PLAYER Methods

// DECK Methods
void DECK_init( DECK *deck );
void DECK_shuffle( DECK *deck );
CARD DECK_remove_card( DECK* deck );
CARD PLAYER_HAND_remove_card(PLAYER* card, int pos);

// Shared Memory Methods
GAMEINFO *shmM_create( char *shm_name, int shm_size );
GAMEINFO *shmM_attach( char *shm_name, int shm_size );
void shmM_destroy( GAMEINFO *shm, char *shm_name, int shm_size );
