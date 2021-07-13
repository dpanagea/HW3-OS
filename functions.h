#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>

// semaphore and shared memory
#include "shm.h"
#include "sems.h"


// semaphore keys
#define WRITERS_KEY  1111
#define RMUTEX_KEY 2222
#define READERS_KEY 3333

// shared memory key
#define KEY 333


void perror_exit(const char *message);

//int get_random(time_t t, int start, int end);

#endif // __FUNCTIONS__
