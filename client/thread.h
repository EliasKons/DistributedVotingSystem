#include "defines.h"

struct thread_args {
    char *serverName, line[MAX_RESPONSE];
    int portNum;
};

void processLine(char *line, int portNum, char *serverName, pthread_t *threads, int numLine);
void *threadFunction(void *arg);