#include "functions.h"

/* ./child m
m -> The size of the array in feeder
*/

int main(int argc, char *argv[])
{
    /*-------------Parsing and Declarations--------------*/
    
    if(argc != 3)
    {
        printf("Wrong arguments. Exiting feeder.. \n");
        exit(-1);
    }
    int m, n;
    m = atoi(argv[1]);
    n = atoi(argv[2]);

    int i, nums[n]; 
    struct timeval tv;
    unsigned long long now, timestamp, avg;
    FILE *fp = fopen("output.txt", "a");
    if (fp == NULL)
        perror("Error opening file \n");
    avg = 0;

    int flags[m];
    for(i = 0; i < m; i++)
        flags[i] = 0;

   
    /*----------------------Semaphores------------------*/
    int writers_sem = semget((key_t)WRITERS_KEY, 1, 0666);
//    int readers_sem = semget((key_t)READERS_KEY, 1, 0666);
    int rmutex      = semget((key_t)RMUTEX_KEY, 1, 0666);

    /*----------Attaching the shared Memory------------*/
    int segment_id = shm_get(KEY);
    entry *shared = (entry*)shm_attach(segment_id);

    /*---------------Starting reading-----------------*/
    while (1)
    {
        if(flags[shared->written] == 0) 
        {
            sem_down(rmutex);

            // CRITICAL SECTION //
            shared->readcount++;
         //   printf("Child #%d, num: %d, rc: %d, flag: %d, i is: %d \n", getpid(), shared->number, shared->readcount, flags[shared->written], shared->written);
            
            sem_up(rmutex);
            gettimeofday(&tv, NULL);
            now = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;
            timestamp = now - shared->timestamp;
            avg+= timestamp;
            nums[shared->written] = shared->number;
            /* PROSOXH: Otan m kai n exoun megalh diafora, kollane parakatw: */
            flags[shared->written]++;            
            /* EXIT SECTION */
            sem_down(rmutex);
            if(shared->readcount == n ) 
                sem_up(writers_sem); 
            else 
                sem_up(rmutex); 
        }
         if(shared->written == m) 
            break;
        if(shared->terminate == 1)
            break;
    }
    fprintf(fp, "PID: %d   OUTPUT: \n", getpid());
    fprintf(fp, "-------------\n");
    for(i = 0; i< m; i++)
        fprintf(fp, "%d \n", nums[i]);
    fprintf(fp, "PID: %d has Running Average: %llu miliseconds \n", getpid(), avg);
    fprintf(fp, "\n \n");
    printf("PID: %d has Running Average: %llu miliseconds \n", getpid(), avg);

    /* -------Detaching the shared memory------- */
    shm_detach((void*)shared);
    fclose(fp);

    return 0;
}