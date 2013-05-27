#include "dataStructs.h"
#include "shmModule.h"

int main(int argc, char *argv[]){

	if(argc != 4){
		printf("Usage: %s <player_name> <shm_name> <n_players>", argv[0]);
		perror("Wrong usage of the program!");
		exit(1);
	}

	

	return 0;
}
