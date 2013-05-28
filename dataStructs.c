#include "dataStructs.h"

char *itoa(int n) {

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

void DECK_init(DECK *deck) {

	int i;
	int suit = 0;
	int rank = FIRST_NUMERAL;
	char *sRank;
	
	deck->size = 52;
	
	for(i = 0; i< DECK_SIZE; i++){

		if((i % 13) == 0)
			strncpy(deck->cards[i].rank, "A\0", 2);
		else if((i % 13) == 10)
			strncpy(deck->cards[i].rank, "J\0", 2);
		else if((i % 13) == 11)
			strncpy(deck->cards[i].rank, "Q\0", 2);
		else if((i % 13) == 12)
			strncpy(deck->cards[i].rank, "K\0", 2);
		else{
			sRank = itoa(rank);
			strncpy(deck->cards[i].rank, sRank, strlen(sRank) + 1);
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
}

void DECK_shuffle(DECK *deck) {

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

CARD DECK_remove_card(DECK* deck) {

	if(deck->cards == NULL || deck->size < 1) exit(-1);

	deck->size--;

	return deck->cards[deck->size];
}

GAMEINFO *shmM_create( char *shm_name, int shm_size ) {

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

GAMEINFO *shmM_attach( char *shm_name, int shm_size ) {

	int shmfd;
	GAMEINFO *shm;

	// Creating the shared memory region //
	// User and Group READ | WRITE permission //
	shmfd = shm_open( shm_name, O_RDWR, 0660);

	if( shmfd < 0 ) {
		// perror("Failure in shm_open()");
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
