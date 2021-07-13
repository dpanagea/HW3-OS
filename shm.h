#ifndef __SHM_H__
#define __SHM_H__

#define SZ 1024

#include "functions.h"

// Entry of the Shared Memory
typedef struct entry
{
    int number;
    unsigned long long timestamp;
    int readcount;
    int written;
    int terminate;
  }entry;

entry* entry_create();
void entry_destroy(entry** e);

// Shared Memory Functions
struct shmid_ds shmbuffer;

int shm_create();
int shm_get(int);
void *shm_attach(int);
void shm_detach(void *);
void shm_destroy(int);

#endif
