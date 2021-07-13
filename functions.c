#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

void perror_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

/*
int get_random(time_t t, int start, int end)
{
    int num;
    
    num = (rand() % (end - start + 1)) + start; 
    return num;
}
*/
