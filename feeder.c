#include "functions.h"
/*
Execute ->  ./feeder <M> <n>
M      ->  size of array in Feeder
n       ->  number of children processes to be forked
*/

int main(int argc, char *argv[])
{
    /*--------Parcing & Declarations---------------*/
    FILE *fp = fopen("output.txt", "a");
    if (fp == NULL)
        perror("Error opening file \n");

    time_t t;
    int i, j, m, n;
    srand(time(0));
    struct timeval tv;
    unsigned long long timestamp;
    gettimeofday(&tv, NULL);
    timestamp = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;

    if (argc != 3)
    {
        printf("Wrong arguments. \n Exiting... \n");
        return -1;
    }
    m = atoi(argv[1]);
    n = atoi(argv[2]);

    int nums[m];

    /*----------------Keeping children's pids--------------------------*/
    pid_t child[n];
    pid_t pid;

    /*---------Semaphores (Creating and initializing them)------------*/
    int writers_sem = semget((key_t)WRITERS_KEY, 1, 0666 | IPC_CREAT);
//    int readers_sem = semget((key_t)READERS_KEY, 1, 0666  |IPC_CREAT);
    int rmutex = semget((key_t)RMUTEX_KEY, 1, 0666 | IPC_CREAT);

    sem_initialize_up(writers_sem);
//    sem_initialize_down(readers_sem);
    sem_initialize_down(rmutex);

    /*--------------Creating the shared memory---------------------*/
    int segment_id = shm_create(KEY);
    entry *shared = (entry*)shm_attach(segment_id);
    
    for(i=0; i< m; i++)
        nums[i] = rand() % 100;
    /*---------------Creating the children-----------------------*/
    char* args[4], sz[10], charn[10], pos[10]; 
    for(i = 0; i < n; i++)
    {
        pid = fork();
        switch (pid)
        {
            case 0:
                args[0] = "./child";
                sprintf(sz, "%d", m);
                args[1] = sz;
                sprintf(charn, "%d", n);
                args[2] = charn;
                args[3] = NULL;

                execvp(args[0], &args[0]);
                exit(EXIT_FAILURE);
            case -1:
                perror_exit("Feeder fork \n");
                return 0;
            default:
                child[i] = pid;
    //            printf("Forked a child with PID: %d \n", pid);
                break;
        }
    }
    printf("\n");

    /*-----------------Being a writer-------------------*/
    shared->terminate = 0;
    shared->written = -1;
    for (i=0; i<m; i++)
    {
        sem_down(writers_sem);
        //Starts writing at the Shared Memory
        //CRITICAL SECTION - ACTUAL WRITING
        shared->number = nums[i];
        shared->readcount = 0;
        shared->written++;
        gettimeofday(&tv, NULL);
        timestamp = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;
        shared->timestamp = timestamp;
        //EXIT SECTION      
        sem_up(rmutex);
    }
    shared->terminate = 1;



    /*----------Wait for child processes to terminate--------------*/
    int stat_val;
    pid_t c_pid;
    
    while(1)
    {
        c_pid = wait(&stat_val);
        if (c_pid < 0)
            break;

        for(i=0; i<n; i++)
        {
            if(c_pid == child[i])
            {
                printf("Child #%d with pid %d has terminated. \n",i, c_pid);
            }
        }
        if(!WIFEXITED(stat_val))
            printf("PID %d -> terminated abnormally \n", c_pid);
    }

    /*--Releasing the shared memory--*/
    shm_destroy(segment_id);

    printf("All are dead. Me dies now \n");

    fclose(fp);

    return 0;

    
}