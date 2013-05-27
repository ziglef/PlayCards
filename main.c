#include "dataStructs.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){

	if(argc != 4){
		printf("Usage: %s <player_name> <shm_name> <n_players>\n", argv[0]);
		exit(1);
	}

	char *pName = strndup( argv[1], strlen(argv[1]) );
	
	char *FIFOname = (char *)malloc( sizeof(char) * strlen(argv[1]) + 2); // 1 for '\0' and 1 for /
	FIFOname[0] = '/';
	strcat(FIFOname, argv[1]);
	
	char *shmName = (char *)malloc( sizeof(char) * strlen(argv[2]) + 2); // 1 for '\0' and 1 for /
	shmName[0] = '/';
	strcat(shmName, argv[2]);
	
	int nPlayers = atoi(argv[3]);
	
	GAMEINFO *shm = shmM_open_create( shmName, sizeof(GAMEINFO) );
	
	if( shm->nPlayers == 0 )
		shmM_init(shm, nPlayers);
		
	int FIFOfd = mkfifo( FIFOname, S_IRWXU );
	
	
	close(FIFOfd);
	unlink(FIFOname);
	shmM_destroy( shm, shmName, sizeof(GAMEINFO) );
	return 0;
}
