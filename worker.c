#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include "worker.h"
#include "error_handler.h"
#include "defines.h"
#include "info.h"

extern pthread_mutex_t bufferMutex, dataMutex;
extern pthread_cond_t bufferNotFull, bufferNotEmpty;
extern int finish;

static int send_and_receive(int newsock, const char *sent, char *received) {
    int bytesWritten;
    size_t sent_len = strlen(sent);

    if (write(newsock, sent, sent_len) == -1) // write message
        perror_exit("write");

    bytesWritten = read(newsock, received, MAX_RESPONSE - 1); // read bytes from client

    if (bytesWritten == -1)
        perror_exit("read");

    else if (bytesWritten == 0) // connection closed by client
        return 1;

    received[bytesWritten] = '\0';

    for (int i = 0; i < bytesWritten; i++) {
        if (received[i] == '\n' || received[i] == '\r') {
            received[i] = '\0';
            break;
        }
    }

    return 0;
}

void* workerThread(void* arg) {
    int newsock;
    char name[MAX_RESPONSE], party[MAX_RESPONSE];
    const char *message1 = "SEND NAME PLEASE\n", *message2 = "SEND VOTE PLEASE\n";
    struct info *t_info = (struct info*)arg;

    while(finish != 1) {
        pthread_mutex_lock(&bufferMutex); // lock bufferMutex

        while(t_info->count == 0) { // buffer is empty
            pthread_cond_wait(&bufferNotEmpty, &bufferMutex); // wait for buffer to not be empty
            if(finish == 1)
                break;
        }

        if(finish == 1) {
            pthread_mutex_unlock(&bufferMutex);
            break;
        }
        
        // bounded buffer client implementation 
        newsock = t_info->buffer[t_info->start];
        t_info->start = (t_info->start + 1) % t_info->size;
        t_info->count--;

        pthread_cond_signal(&bufferNotFull);

        pthread_mutex_unlock(&bufferMutex);

        if(send_and_receive(newsock, message1, name) == 1) {
            close(newsock);
            continue;
        }

        pthread_mutex_lock(&dataMutex);

        if(search_vote(t_info->votes, name) == 1) {
            printf("ALREADY VOTED\n");
            pthread_mutex_unlock(&dataMutex);
            close(newsock);
            continue;
        } 

        pthread_mutex_unlock(&dataMutex);
        
        if(send_and_receive(newsock, message2, party) == 1) {
            close(newsock);
            continue;
        }

        pthread_mutex_lock(&dataMutex);

        insert_vote(t_info->votes, name, party); 
        add_vote(t_info->results, party);

        fprintf(t_info->pollLog, "%s %s\n", name, party);

        pthread_mutex_unlock(&dataMutex);

        printf("VOTE for Party %s RECORDED\n", party);

        close(newsock);
    }

    return NULL;
}