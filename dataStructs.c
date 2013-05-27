#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#include "dataStructs.h"

char *itoa(int n){

	char *c = (char *)malloc(sizeof(char));
	int i = 0;

	while(n != 0){
		c[i] = (n % 10) + 48;
		n = n / 10;
		i++;
		c = (char *)realloc(c, sizeof(char) * (i + 1));
	}

	int j, k;
	char aux;
	for(j=0, k=i-1; k>=(i/2); j++, k--){
		aux = c[j];
		c[j] = c[k];
		c[k] = aux;
	}

	c[i] = '\0';

	return c;
}


DECK *DECK_init(){

	int i;
	int suit = 0;
	int rank = FIRST_NUMERAL;
	char *sRank;
	
	DECK *deck = (DECK *)malloc( sizeof(DECK) );
	
	if( deck == NULL )
		return NULL;
		
	deck->cards = (CARD *)malloc( sizeof(CARD) * DECK_SIZE );
	if( deck->cards == NULL )
		return NULL;
	
	for(i = 0; i< DECK_SIZE; i++){

		if((i % 13) == 0)
			deck->cards[i].rank = strndup("A", 1);
		else if((i % 13) == 10)
			deck->cards[i].rank = strndup("J", 1);
		else if((i % 13) == 11)
			deck->cards[i].rank = strndup("Q", 1);
		else if((i % 13) == 12)
			deck->cards[i].rank = strndup("K", 1);
		else{
			sRank = itoa(rank);
			deck->cards[i].rank = strndup(sRank, strlen(sRank));
			rank++;
		}

		switch(suit){
		case 0: deck->cards[i].suit = 'c';	break;	// Clubs
		case 1: deck->cards[i].suit = 'd';	break;	// Diamonds
		case 2: deck->cards[i].suit = 'h';	break;	// Hearts
		case 3: deck->cards[i].suit = 's';	break;	// Spades
		}

		if(i==12 || i == 25 || i == 38){
			suit++;
			rank = FIRST_NUMERAL;
		}
	}
	
	return deck;
}

void DECK_shuffle(DECK *deck){

	int i, j, k;

	CARD aux;

	for(i = 0; i < SHUFFLE_TIMES; i++){

		do{
			k = rand() % 52;
			j = rand() % 52;
		}while(i == j);

		aux = deck->cards[j];
		deck->cards[j] =  deck->cards[k];
		deck->cards[k] = aux;
	}
}

GAMEINFO *shmM_open_create( char *shm_name, int shm_size ) {

	int shmfd;
	GAMEINFO *shm;

	// Creating the shared memory region //
	// User and Group READ | WRITE permission //
	shmfd = shm_open( shm_name, O_CREAT | O_RDWR, 0660);

	if( shmfd < 0 ) {
		perror("Failure in shm_open()");
		return NULL;
	}

	if( ftruncate( shmfd, shm_size ) < 0 ) {	// Truncate the memory to its size //
		perror("Failure in ftruncate()");
		return NULL;
	}

	// Maps the memory to the GAMEINFO struct //
	shm = mmap( 0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
	if( shm == MAP_FAILED ) {
		perror("Failure in mmap()");
		return NULL;
	}

	return (GAMEINFO *)shm;
}

void shmM_destroy(GAMEINFO *shm, char *shm_name, int shm_size) {

	if( munmap( shm, shm_size ) < 0 ) {
		perror("Failure in munmap()");
		exit(EXIT_FAILURE);
	}

	if( shm_unlink( shm_name ) < 0 ) {
		perror("Failure in shm_unlink()");
		exit(EXIT_FAILURE);
	}
}

// TESTING PURPOSES ONLY //
/*
int main(){

	int i;
	srand(time(NULL));
	
	DECK *deck = DECK_init();

	printf("** Before Shuffles **\n");
	for(i=0;i<52;i++){
		printf("%s%c  ", deck->cards[i].rank, deck->cards[i].suit);

		if((i % 13) == 12)
			printf("\n");
	}

	printf("\n** After First Shuffle **\n");
	DECK_shuffle(deck);

	for(i=0;i<52;i++){
		printf("%s%c  ", deck->cards[i].rank, deck->cards[i].suit);

		if((i % 13) == 12)
			printf("\n");
	}


	printf("\n** After Second Shuffle **\n");

	DECK_shuffle(deck);

	for(i=0;i<52;i++){
		printf("%s%c  ", deck->cards[i].rank, deck->cards[i].suit);

		if((i % 13) == 12)
			printf("\n");
	}

	return 0;
}
*/
