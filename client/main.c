#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include "error_handler.h"
#include "defines.h"
#include "thread.h"


int main(int argc, char *argv[]) {
    int portNum, numLines = 0, numLine = 0;
    char line[MAX_RESPONSE];
    FILE *file;
    pthread_t *threads;

    if(argc != 4) {
        printf("Wrong argument usage.\n");
        exit(1);
    }

    if((file = fopen(argv[3], "r")) == NULL) {
        fprintf(stderr, "Error opening %s file\n", argv[3]);
        exit(1);
    }

    portNum = atoi(argv[2]); // port number given to the server


    // number of lines of the file, used in order to create "clients"
    while (fgets(line, sizeof(line), file) != NULL)  
        numLines++;
    
    rewind(file); // Reset the file pointer to the beginning

    threads = malloc(numLines * sizeof(pthread_t)); // "voters" creation
    

    while(fgets(line, sizeof(line), file) != NULL) {
        if(line[strlen(line) - 1] == '\n') 
            line[strlen(line) - 1] = '\0';

        processLine(line, portNum, argv[1], threads, numLine); // process the line of the file
        numLine++;
    }

    // wait for all the clients to finish
    for(int i = 0; i < numLines; i++) 
        pthread_join(threads[i], NULL);
    

    fclose(file);

    free(threads);

    return 0;

}