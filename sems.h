#ifndef __SEMS_H__
#define __SEMS_H__

#include "functions.h"

// SEMAPHORE FUNCTIONS
 union semun1 
 {
     int val;
     struct semid_ds *buf;
     unsigned short *array;
};

void sem_initialize_up(int);
void sem_initialize_down(int);
void sem_destroy(int);
void sem_up(int);
void sem_down(int);

#endif
