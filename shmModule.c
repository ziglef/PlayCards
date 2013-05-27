#include "dataStructs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct {

	PLAYER *players;
	int nplayers;
	int pturn;
	int round;
	int dealer;
	CARD *table;
} GAMEINFO;
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

void *shmM_destroy(GAMEINFO *shm, char *shm_name, int shm_size) {

	if( munmap( shm, shm_size ) < 0 ) {
		perror("Failure in munmap()");
		exit(EXIT_FAILURE);
	}

	if( shm_unlink( shm_name ) < 0 ) {
		perror("Failure in shm_unlink()");
		exit(EXIT_FAILURE);
	}
}


