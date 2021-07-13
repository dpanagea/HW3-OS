#include "sems.h"


// SEMAPHORE FUNCTIONS
void sem_initialize_up(int sem_id)
{
    union semun1 sem_union;
    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) 
        perror_exit("Set Semvalue (up) \n");
}

void sem_initialize_down(int sem_id)
{
    union semun1 sem_union;
    sem_union.val = 0;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) 
        perror_exit("Set Semvalue (down) \n");
}

void sem_destroy(int sem_id)
{
    union semun1 sem_union;
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) 
        perror_exit("Delete Semaphore \n");
}

void sem_up( int sem_id )
{
    struct sembuf sem_b = {0, 1, 0};   // V()
    if (semop(sem_id, &sem_b, 1) == -1) 
        perror_exit("Semaphore V(up) \n");
}

void sem_down(int sem_id)
{
    struct sembuf sem_b = {0, -1, 0};  // P()
    if (semop(sem_id, &sem_b, 1) == -1) 
        perror_exit("Semaphore P(down) \n");
}
