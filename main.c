#include "dataStructs.h"

void *gameLoop( void *arg ){
	
	GAMEINFO *shm = arg;
	
	int i, j;
	int FIFOS[shm->nPlayers];
	int NPLAYERS = shm->nPlayers;
	
	printf("OPENING FIFOS!\n");
	fflush(stdout);
	for(i=0; i<NPLAYERS; i++) FIFOS[i] = open( shm->players[i].FIFOname, O_RDWR | O_CREAT, S_IRWXU );

	printf("LOCKING MUTEXES!\n");
	fflush(stdout);
	for(i=0; i<NPLAYERS; i++) pthread_mutex_lock( &shm->PPERM[i] );

	pthread_mutex_unlock( &shm->GAMESTART_MUT );

	printf("DECK SHUFFLING\n");
	fflush(stdout);
	DECK_shuffle( &shm->table );
	
	for(i=0; i<NPLAYERS; i++){
		printf("DEALING CARDS TO PLAYER %d\n", i+1);
		for(j=0; j<DECK_SIZE/NPLAYERS; j++){
			CARD buf = DECK_remove_card( &shm->table );
			write( FIFOS[i], &buf, sizeof(CARD) );
		}
	}
	
	printf("UNLOCKING MUTEXES!\n");
	fflush(stdout);
	for(i=0; i<NPLAYERS; i++) pthread_mutex_unlock( &shm->PPERM[i] );
	
	sleep(1);
	
	printf("LOCKING MUTEXES!\n");
	fflush(stdout);
	for(i=0; i<NPLAYERS; i++) pthread_mutex_lock( &shm->PPERM[i] );
	
	sleep(2);
	printf("SHM->PTURN : %d\n", shm->pturn + 1);
	pthread_mutex_unlock( &shm->PPERM[shm->pturn + 1] );
	sleep(3);
	
	printf("SETTING GAMERUNNING TO 0!\n");
	// pthread_mutex_lock( &shm->GAMEFLAGS_MUT );
	shm->GAMERUNNING = 0;
	// pthread_mutex_unlock( &shm->GAMEFLAGS_MUT );
	sleep(1);
	for(i=0; i<NPLAYERS; i++) pthread_mutex_unlock( &shm->PPERM[i] );
	
	printf("CLOSING FIFOS!\n");
	for(i=0; i<NPLAYERS; i++) close( FIFOS[i] );
	
	return 0;
}

int main(int argc, char *argv[]){

	int i;

	if(argc != 4){
		printf("Usage: %s <player_name> <shm_name> <n_players>\n", argv[0]);
		exit(1);
	}
	
	int PLAYERID = 0;

	char *pName = strndup( argv[1], strlen(argv[1]) );
	
	int dealer = 0;
	pthread_t tid;
	
	char *FIFOname = (char *)malloc( sizeof(char) * strlen(argv[1]) + 2); // 1 for '\0' and 1 for /
	FIFOname[0] = '/';
	strcat(FIFOname, argv[1]);
	
	char *shmName = (char *)malloc( sizeof(char) * strlen(argv[2]) + 2); // 1 for '\0' and 1 for /
	shmName[0] = '/';
	strcat(shmName, argv[2]);
	
	int nPlayers = atoi(argv[3]);
	
	GAMEINFO *shm = shmM_attach( shmName, sizeof(GAMEINFO) );
	if( shm == NULL ) shm = shmM_create( shmName, sizeof(GAMEINFO) );
	
	if( shm->currPlayers == 0 ) {
		
		shm->nPlayers = nPlayers;
		shm->currPlayers = 0;
		shm->pturn = 0;
		shm->currRound = 0;
		shm->dealer = 0;
		DECK_init( &shm->table );
		
		pthread_mutexattr_t SHARED_ATTR;
		pthread_mutexattr_init( &SHARED_ATTR );
		pthread_mutexattr_setpshared( &SHARED_ATTR, PTHREAD_PROCESS_SHARED );
		pthread_mutex_init( &shm->GAMEFLAGS_MUT, &SHARED_ATTR );
		pthread_mutex_init( &shm->GAMESTART_MUT, &SHARED_ATTR );
		
		for(i=0; i<PLAYER_MAX; i++){
			pthread_mutex_init( &shm->PPERM[i], &SHARED_ATTR );
		}
		
		shm->GAMERUNNING = 1;
		
		dealer = 1;
	}
	
	unlink( FIFOname );
	
	if( mkfifo( FIFOname, S_IFIFO ) == -1 ) {
		perror("Failure in mkfifo()");
		exit(4);
	}
	
	PLAYERID = shm->currPlayers;
	
	shm->currPlayers += 1;
	shm->players[shm->currPlayers - 1].number = shm->currPlayers - 1;
	shm->players[shm->currPlayers - 1].initialized = 0;
	strncpy( shm->players[shm->currPlayers - 1].name, pName, strlen(pName) + 1 );
	strncpy( shm->players[shm->currPlayers - 1].FIFOname, FIFOname, strlen(FIFOname) + 1);
	
	printf("GAMEINFO :\n");
	for(i = 0; i < shm->currPlayers; i++)
		printf("PLAYER : %s\n", shm->players[i].name);
	printf("CURRENT PLAYERS : %d\n", shm->currPlayers);
	printf("TABLE MAX PLAYERS : %d\n", shm->nPlayers);
	printf("PLAYER TURN : %d\n", shm->pturn);
	printf("ROUND : %d\n", shm->currRound);
	printf("DEALER : %d\n\n", shm->currRound);
	
	if ( dealer == 1 ) { // DEALER CODE
		pthread_mutex_lock( &shm->GAMESTART_MUT );
		while(1) {
			pthread_mutex_lock( &shm->GAMEFLAGS_MUT );
			if ( shm->currPlayers == shm->nPlayers ) break;
			pthread_mutex_unlock( &shm->GAMEFLAGS_MUT );
		}
		
		if( pthread_create( &tid, NULL, gameLoop, shm ) != 0 ) {
			perror("Failure in pthread_create()");
			exit(2);
		} 
		
		void *THREAD_RETURN;
		pthread_join( tid, &THREAD_RETURN ); 
		
		pthread_mutex_unlock( &shm->GAMEFLAGS_MUT );
		
		shm->currPlayers -= 1;
		
	} else {	// NOT DEALER CODE
		int PFIFO;
		PFIFO = open( shm->players[PLAYERID].FIFOname, O_RDWR | O_CREAT, S_IRWXU );

		CARD buf;
		int C2PLAY;
		
		pthread_mutex_lock( &shm->GAMESTART_MUT );
		printf("IM FUCKING HERE!\n");
		fflush(stdout);
		while(shm->GAMERUNNING){
			pthread_mutex_unlock( &shm->GAMESTART_MUT );			
			pthread_mutex_lock( &shm->PPERM[PLAYERID] );
			if( shm->GAMERUNNING == 0 ) break;
			if( shm->players[PLAYERID].initialized == 0) {
				printf("TRYING TO READ FROM FIFO!\n");
				for( i=0; i<DECK_SIZE/shm->nPlayers; i++ ) {
					read( PFIFO, &buf, sizeof(CARD) );
					shm->players[PLAYERID].hand[i] = buf;
				}
				shm->players[PLAYERID].initialized = 1;
			} else {
				for( i=0; i<DECK_SIZE/shm->nPlayers; i++ )
					printf("CARTA %d : %s%c\n", i, shm->players[PLAYERID].hand[i].rank, shm->players[PLAYERID].hand[i].suit ); 
					
				printf("Introduza o indice da carta a jogar: ");
				scanf("%d", &C2PLAY);
				
				write( PFIFO, &C2PLAY, sizeof(int) );
			}
				
			pthread_mutex_unlock( &shm->PPERM[PLAYERID] );
			sleep(2);
		}
		
		shm->currPlayers -= 1;
		close( PFIFO );
		pthread_mutex_unlock( &shm->PPERM[PLAYERID] );
	}
	
	if( dealer == 1 ){
		// THREAD FREE //
		pthread_detach(tid);
		
		// SHM FREE //
		shmM_destroy( shm, shmName, sizeof(GAMEINFO) );
	} else {
		shmdt(shm);
	}
	
	// FIFO FREE //
	unlink( FIFOname );
	
	// FREE OTHER VARIABLES //
	free( pName );
	free( FIFOname );
	free( shmName );
	
	return 0;
}
