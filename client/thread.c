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


void processLine(char *line, int portNum, char *serverName, pthread_t *threads, int numLine) {
    int err;
    struct thread_args *args = malloc(sizeof(struct thread_args)); // structure used to save important information  

    args->serverName = serverName;
    args->portNum = portNum;
    strcpy(args->line, line);

    if((err = (pthread_create(&threads[numLine], NULL, threadFunction, (void*)args)) != 0)) // "voter" creation
        perror2("pthread_create", err);

}

void *threadFunction(void *arg) {
    int sock;
    unsigned int serverlen;
    struct thread_args *args = (struct thread_args*)arg;
    char name[NAME_SIZE], lastname[NAME_SIZE], party[PARTY_SIZE], data[MAX_RESPONSE], message1[MAX_RESPONSE], message2[MAX_RESPONSE];
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;

    if(args->line[0] == '\0') {
        free(args);
        return NULL;
    }


    sscanf(args->line, "%s %s %s", name, lastname, party); // get name, lastname and party of the voter
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) // socket creation
        perror_exit("socket");

    if ((rem = gethostbyname(args->serverName)) == NULL) { // find the host using his name
        herror("gethostbyname");
        exit(1);
    }

    server.sin_family = AF_INET;

    memcpy(&server.sin_addr, rem->h_addr_list[0], rem->h_length);
    server.sin_port = htons(args->portNum);

    serverlen = sizeof server;


    if (connect(sock, serverptr, serverlen) < 0) // connect to the server
        perror_exit("connect");

    snprintf(data, sizeof(data), "%s %s", name, lastname);

    if (read(sock, message1, MAX_RESPONSE - 1) == -1) // name message
        perror_exit("read");

    if (write(sock, data, strlen(data)) < 0) // pass name to the server
        perror_exit("write");

    if (read(sock, message2, MAX_RESPONSE - 1) == -1) // party message
        perror_exit("read");

    if (write(sock, party, strlen(party)) < 0) // pass party to the server
        perror_exit("write");


    close(sock);
    free(args);

    return NULL;
}
