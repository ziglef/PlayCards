#include "dataStructs.h"

void *gameLoop( void *arg ){
	return NULL;
}

int main(int argc, char *argv[]){

	if(argc != 4){
		printf("Usage: %s <player_name> <shm_name> <n_players>\n", argv[0]);
		exit(1);
	}

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
	
	GAMEINFO *shm = shmM_create( shmName, sizeof(GAMEINFO) );
	if( shm == NULL ) shm = shmM_attach( shmName, sizeof(GAMEINFO) );
	
	if( shm->currPlayers == 0 ) {
		
		shm->nPlayers = nPlayers;
		shm->currPlayers = 0;
		shm->pturn = 0;
		shm->currRound = 0;
		shm->dealer = 0;
		pthread_mutex_init( &(shm->gameStart_mut), NULL );
		
		dealer = 1;
	}
	
	if( mkfifo( FIFOname, S_IRWXU ) == -1 ) {
		perror("Failure in mkfifo()");
		exit(4);
	}
	
	shm->currPlayers += 1;
	shm->players[shm->currPlayers - 1].number = shm->currPlayers - 1;
	strncpy( shm->players[shm->currPlayers - 1].name, pName, strlen(pName) + 1 );
	strncpy( shm->players[shm->currPlayers - 1].FIFOname, FIFOname, strlen(FIFOname) + 1);
	
	int i;
	printf("GAMEINFO :\n");
	for(i = 0; i < shm->currPlayers; i++)
		printf("PLAYER : %s\n", shm->players[i].name);
	printf("CURRENT PLAYERS : %d\n", shm->currPlayers);
	printf("TABLE MAX PLAYERS : %d\n", shm->nPlayers);
	printf("PLAYER TURN : %d\n", shm->pturn);
	printf("ROUND : %d\n", shm->currRound);
	printf("DEALER : %d\n", shm->currRound);
	
	if ( dealer == 1 ) { // DEALER CODE
		while(1) {
			pthread_mutex_lock( &shm->gameStart_mut );
			if ( shm->currPlayers == shm->nPlayers )
				break;
			pthread_mutex_unlock( &shm->gameStart_mut );
		}
		
		printf("WE DID IT\n");
		pthread_mutex_unlock( &shm->gameStart_mut ); 
	
		if( pthread_create( &tid, NULL, gameLoop, shm ) != 0 ) {
			perror("Failure in pthread_create()");
			exit(2);
		} 
	} else {	// NOT DEALER CODE
		printf("NOT DEALER\n");
	}
	
	for(i = 0; i < shm->currPlayers; i++){
		printf("Hi, i am player %s\n", shm->players[i].name);
		fflush(stdout);
	}
	
	getchar();
	
	shm->currPlayers -= 1;
	
	if( dealer == 1 ){
		// THREAD FREE //
		pthread_detach(tid);
	}
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
