#include "shm.h"

//Entry Shared Memory Functions
entry* entry_create()
{
    entry* e = malloc(sizeof(entry));
    if (e == NULL)
        return NULL;
    e->number = 0;
    e->timestamp = 0;
    e->readcount = 0;
    return e;
}

void entry_destroy(entry** e)
{
    if(*e)
    {
        free(*e);
        e = NULL;
    }
}



// Shared Memory Functions
int shm_create(int key)
{
    int id = shmget( (key_t)key, SZ, IPC_CREAT | 0666 ); 
    if (id < 0) 
        perror_exit("Create shm \n");

    return id;
}

int shm_get(int key)
{
    int id = shmget( (key_t)key, SZ, 0666 );
    if (id < 0) 
        perror_exit("Get shm \n");

    return id;
}

void *shm_attach(int id)
{
    void *data = shmat(id, (void *)0, 0);
    if ((char *)data == (char *)-1) 
        perror_exit("Attach shm \n");

    return data;
}

void shm_detach(void *segment)
{
    if (shmdt(segment) < 0) 
        perror_exit("Detach Segment \n");

}

void shm_destroy(int id)
{
    if (shmctl(id, IPC_RMID, 0) < 0)
        perror_exit("Destroy shm \n"); 
}
