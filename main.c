#include "dataStructs.h"

void *gameLoop( void *arg ){
	// printf("HELLO\n");
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
	
	GAMEINFO *shm = shmM_attach( shmName, sizeof(GAMEINFO) );
	if( shm == NULL ) shm = shmM_create( shmName, sizeof(GAMEINFO) );
	
	if( shm->currPlayers == 0 ) {
		shm->nPlayers = nPlayers;
		dealer = 1;
	}
	
	int FIFOfd = mkfifo( FIFOname, S_IRWXU );
	
	PLAYER_init( &(shm->players[shm->currPlayers - 1]), pName, FIFOname, shm->nPlayers - 1, FIFOfd );	
	
	printf("ANTES DO LOCK");
	fflush(stdout);
	
	pthread_mutex_lock( &shm->gameStart_mut );
	printf("ANTES DO LOCK");
	fflush(stdout);
	shm->currPlayers++;
	printf("ANTES DO LOCK");
	fflush(stdout);
	pthread_cond_signal( &shm->gameStart_cond );
	pthread_mutex_unlock( &shm->gameStart_mut );
	
	printf("DEPOIS DO LOCK");
	fflush(stdout);
	
	if ( dealer == 1 ) { // DEALER CODE
		pthread_mutex_lock( &shm->gameStart_mut );
		while( shm->currPlayers != shm->nPlayers ) {
			printf("CURR: %d\nPLAYERS: %d\n", shm->currPlayers, shm->nPlayers);
			fflush(stdout);
			pthread_cond_wait( &shm->gameStart_cond, &shm->gameStart_mut );
			printf("SIGNAL RECEIVED!\n");
		}
		pthread_mutex_unlock(&shm->gameStart_mut);

		if( pthread_create( &tid, NULL, gameLoop, shm ) != 0 ) {
			perror("Failure in pthread_create()");
			exit(2);
		}

		pthread_mutex_unlock( &(shm->gameStart_mut) ); 
	} else {	// NOT DEALER CODE
		printf("NOT DEALER\n");
	}
	
	int i;
	for(i = 0; i < shm->currPlayers; i++){
		printf("Hi, i am player %s\n", shm->players[i].name);
		fflush(stdout);
	}
	
	getchar();
	
	shm->currPlayers--;
	
	//if( dealer == 1 ){
		// THREAD FREE //
		pthread_detach(tid);
	
		// SHM FREE //
		shmM_destroy( shm, shmName, sizeof(GAMEINFO) );
	//} else {
		shmdt(shm);
	//}
	
	// FIFO FREE //
	close( FIFOfd );
	unlink( FIFOname );
	
	// FREE OTHER VARIABLES //
	free( pName );
	free( FIFOname );
	free( shmName );
	
	return 0;
}
